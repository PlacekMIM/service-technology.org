/*****************************************************************************\
 Marlene -- Synthesis of behavioral adapters

 Copyright (C) 2009  Christian Gierds <gierds@informatik.hu-berlin.de>

 Marlene is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 Marlene is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with Marlene.  If not, see <http://www.gnu.org/licenses/>. 
\*****************************************************************************/

// include standard C++ headers
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

// include Marlene's own headers
#include "config.h"
#include "subversion.h"
#include "cmdline.h"
#include "helper.h"
#include "macros.h"
#include "adapter.h"

// include PN Api headers
#include "pnapi/pnapi.h"
// #include "pnapi/parser.h"

int main(int argc, char* argv[])
{
    std::vector< pnapi::PetriNet * > nets;
    
    /*************************************************\
    * Parsing the command line and evaluation options *
    \*************************************************/
    // print debug information about system variables, the current compilation
    // in case of unusal behavior of Fiona, with this information it might be 
    // easier to track down the bug(s)
    if (argc == 2 && std::string(argv[1]) == "--bug") {
        printf("\n\n");
        printf("Please email the following information to %s:\n", PACKAGE_BUGREPORT);
        printf("- tool:               %s\n", PACKAGE_NAME);
        printf("- version:            %s\n", PACKAGE_VERSION);
        printf("- compilation date:   %s\n", __DATE__);
        printf("- compiler version:   %s\n", __VERSION__);
        printf("- platform:           %s\n", CONFIG_BUILDSYSTEM);
#if defined(SVNREV)
        printf("- SVN revision:       %s\n", SVNREV);
#endif
        //printf("- config ASSERT:      %s\n", CONFIG_ENABLEASSERT);
        //printf("- config UNIVERSAL:   %s\n", CONFIG_ENABLEUNIVERSAL);
        //printf("- config ENABLE64BIT: %s\n", CONFIG_ENABLE64BIT);
        //printf("- config WIN32:       %s\n", CONFIG_ENABLEWIN32);
//      printf("- external dot:       %s\n", CONFIG_DOT);
        printf("\n\n");

        exit(0);
    }
    // evaluate command line options
    evaluate_command_line(argc, argv);

    // store invocation in a string for meta information in file output
    std::string invocation;
    for (int i = 0; i < argc; ++i) {
        invocation += std::string(argv[i]) + " ";
    }
    
    /******************\
    * Reading all nets *
    \******************/
    // if nets given as arguments read from files
    if ( args_info.inputs_num > 0 )
    {
        for ( unsigned i = 0; i < args_info.inputs_num; ++i )
        {
            pnapi::PetriNet * net = new pnapi::PetriNet();
            std::string filename = args_info.inputs[i];
            std::cerr << "file: " << filename << std::endl;
            try {
                std::ifstream inputfile(filename.c_str(), std::ios_base::in);
                if (inputfile.good())
                {
                    inputfile >> meta(pnapi::io::INPUTFILE, filename)
                    >> meta(pnapi::io::CREATOR, PACKAGE_STRING)
                    >> meta(pnapi::io::INVOCATION, invocation) >> pnapi::io::owfn >> *net;
                    nets.push_back(net);
                    //std::cout << pnapi::io::dot << *net;

                }
                else
                {
                    abort(2, "Open net file %s could not be opened for reading", filename.c_str());
                }
            } catch (pnapi::io::InputError error) {
                std::cerr << PACKAGE << ":" << error << std::endl;
                abort(2, " ");
                //infile.close();
                exit(EXIT_FAILURE);
            }

        }
    }
    else
    { //! parsing open net for stdin aka std::cin
        pnapi::PetriNet * net = new pnapi::PetriNet();
        nets.push_back(net);
        try {
            std::cin >> pnapi::io::owfn >> *net;
        } catch (pnapi::io::InputError error) {
            std::cerr << PACKAGE << ":" << error << std::endl;
            abort(2, " ");
            //infile.close();
            exit(EXIT_FAILURE);
        }
    }
    
    /*************************\
    * All necessary variables *
    \*************************/
    
    //! #rs contains a RuleSet that is provided for the adapter
    RuleSet rs;
    //! #adapter
    Adapter adapter(nets, rs, contType, messageBound);

    if ( args_info.rulefile_given )
    {
        FILE * rulefile (NULL);
        if((rulefile = fopen(args_info.rulefile_arg,"r")))
        {
            rs.addRules(rulefile);
            fclose(rulefile);
            rulefile = NULL;
        }
        else
        {
            abort(2, "Rule file %s could not be opened for reading", args_info.rulefile_arg);
        }
    }
    
    pnapi::PetriNet engine (*adapter.buildEngine());
    pnapi::PetriNet controller (*adapter.buildController());
    
    engine.compose(controller, "engine", "controller");
    
    engine.reduce(pnapi::PetriNet::LEVEL_4);
    std::cout << pnapi::io::owfn << engine;

    /*******************\
    * Deleting all nets *
    \*******************/
    for ( unsigned int index = 0; index < nets.size(); ++index)
    {
        delete nets[index];
    }
    nets.clear();
    
    return 0;
}

