/*****************************************************************************\
 ProductOG -- Computing product OGs

 Copyright (C) 2009  Christian Sura <christian.sura@uni-rostock.de>

 ProductOG is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 ProductOG is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with ProductOG.  If not, see <http://www.gnu.org/licenses/>. 
\*****************************************************************************/


#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <queue>


#include <config.h>
#include "cmdline.h"
#include "util.h"


using std::cerr;
using std::cout;
using std::endl;
using std::ofstream;
using std::map;
using std::vector;
using std::string;
using std::set;
using std::queue;


/// the command line parameters
gengetopt_args_info args_info;


/// lexer and parser
extern int yyparse();
extern int yylex_destroy();
extern FILE* yyin;


/// output stream
std::ostream* myOut = &cout;

/****************************
 * Variables used by parser *
 ***************************/

/// successor list of recent OG
map<unsigned int, map<string, unsigned int> > succ;
/// annotations of recent OG
map<unsigned int, string> formulae;
/// input labels of recent OG
set<string> inputs;
/// output labels of recent OG
set<string> outputs;
/// synchronous labels of recent OG
set<string> synchronous;
/// present labels of recent OG
map<unsigned int, set<string> > presentLabels;
/// initial node ID of recent OG
unsigned int * initialID = NULL;

/*************************
 * Variables storing OGs *
 ************************/

/// successor lists of all OGs
vector<map<unsigned int, map<string, unsigned int> > > succList;
/// annotations of all OGs
vector<map<unsigned int, string> > formulaeList;
/// input labels of all OGs
vector<set<string> > inputList;
/// output labels of all OGs
vector<set<string> > outputList;
/// synchronous labels of all OGs
vector<set<string> > synchronousList;
/// present labels of all OGs
vector<map<unsigned int, set<string> > > presentLabelList;
/// initial node ID of all OGs
vector<unsigned int> initialIDs;

/********************************
 * Variables storing product OG *
 *******************************/
/// product inputs
set<string> pInputs;
/// product inputs
set<string> pOutputs;
/// product inputs
set<string> pSynchronous;
/// product formulae
map<unsigned int, string> pFormulae;
/// product successors
map<unsigned int, map<string, unsigned int> > pSucc;
/// mapping from ID to state
map<unsigned int, vector<unsigned int> > ID2state;

/// evaluate the command line parameters
void evaluateParameters(int argc, char** argv) 
{
  // initialize the parameters structure
  struct cmdline_parser_params *params = cmdline_parser_params_create();

  // call the cmdline parser
  cmdline_parser(argc, argv, &args_info);

  free(params);
}

/// zero vector needed by stateID()
vector<unsigned int> * zeroVector = NULL;

/// maps a vector of states to state ID
unsigned int stateID(const vector<unsigned int> & state)
{
  static unsigned int newID = 1;
  static map<vector<unsigned int>, unsigned int> IDs;
  
  // initialize zero vector
  if(zeroVector == NULL)
  {
    zeroVector = new vector<unsigned int>();
    for(unsigned int i = 0; i < state.size(); ++i)
    {
      zeroVector->push_back(0);
    }
  }
  
  // the empty node of all OGs will be the empty node of the product OG
  if(state == *zeroVector)
    return 0;
  
  return ((IDs[state] == 0) ? (IDs[state] = newID++) : IDs[state]);
}

/// main function
int main(int argc, char** argv)
{
  /*--------------------------------------.
  | 0. parse the command line parameters  |
  `--------------------------------------*/
  // stream for file output
  ofstream ofs;
  
  // parse commandline
  evaluateParameters(argc, argv);
  
  /*---------------------------.
  | 1. set output destination  |
  `---------------------------*/
  if(args_info.output_given) // if user set an output file
  {
    ofs.open(args_info.output_arg, std::ios_base::trunc); // open file
    if(!ofs) // if an error occurred on opening the file
    {
      cerr << PACKAGE << ": ERROR: failed to open output file '"
           << args_info.output_arg << "'" << endl;
      exit(EXIT_FAILURE);
    }
    
    // else link output stream to file stream
    myOut = &ofs;
  }
  
  /*--------------.
  | 2. parse OGs  |
  `--------------*/
  if(args_info.inputs_num < 2) // if too few OGs are given
  {
    cerr << PACKAGE << ": ERROR: at least 2 OGs must be given" << endl;
    exit(EXIT_FAILURE);
  }
  
  for(unsigned int i = 0; i < args_info.inputs_num; ++i)
  {
    // open file and link input file pointer
    yyin = fopen(args_info.inputs[i], "r");
    if(!yyin) // if an error occurred
    {
      cerr << PACKAGE << ": ERROR: failed to open input file '"
           << args_info.inputs[i] << "'" << endl;
      exit(EXIT_FAILURE);
    }
    
    /// actual parsing
    yyparse();

    /// save parsed information
    succList.push_back(succ);
    formulaeList.push_back(formulae);
    inputList.push_back(inputs);
    outputList.push_back(outputs);
    synchronousList.push_back(synchronous);
    presentLabelList.push_back(presentLabels);
    if(initialID != NULL)
    {
      initialIDs.push_back(*initialID);
    }
    else
    {
      cerr << PACKAGE << ": ERROR: producing with empty OG '"
           << args_info.inputs[i] << "' results in empty product OG" << endl;
      exit(EXIT_FAILURE);
    }
      
    
    /// clear maps
    succ.clear();
    formulae.clear();
    inputs.clear();
    outputs.clear();
    synchronous.clear();
    presentLabels.clear();
    delete initialID;
    initialID = NULL;
    
    /// close input
    fclose(yyin);
  }
  
  /// clean lexer memory
  yylex_destroy();
  
  /*------------------------.
  | 3. compute product OGs  |
  `------------------------*/
  
  // compute new interface
  pInputs = util::intersection(inputList);
  pOutputs = util::intersection(outputList);
  pSynchronous = util::intersection(synchronousList);
  unsigned int stateID_; // will be the ID of the last state
  
  /* A breadth-first search will be startet by the state
   * consisting of the initial states of all OGs
   */
  {
    vector<unsigned int> state = initialIDs; // start with the initial state
    vector<unsigned int> successor = state;
    queue<unsigned int> toDo; // queue of states to do
    stateID_ = stateID(state); 
    toDo.push(stateID_); 
    ID2state[stateID_] = state;
    while(!toDo.empty())
    {
      stateID_ = toDo.front();
      toDo.pop();
      
      state = ID2state[stateID_];
      string formula;
      formula = "(" + formulaeList[0][state[0]] + ")";
      for(unsigned int i = 1; i < state.size(); ++i)
      {
        formula += " * (" + formulaeList[i][state[i]] + ")";
      }
      pFormulae[stateID_] = formula;
      
      vector<set<string> > labels;
      for(unsigned int i = 0; i < state.size(); ++i)
      {
        labels.push_back(presentLabelList[i][state[i]]);
      }
      
      set<string> labels_ = util::intersection(labels);
      for(set<string>::iterator l = labels_.begin(); l != labels_.end(); ++l)
      {
        for(unsigned int i = 0; i < state.size(); ++i)
        {
          successor[i] = succList[i][state[i]][*l];
        }
        
        unsigned int newStateID = stateID(successor);
        pSucc[stateID_][*l] = newStateID;
        if(newStateID > stateID_)
        {
          toDo.push(newStateID);
          ID2state[newStateID] = successor;
        }
      }
    }
  }
  
  /*-----------------.
  | 4. write output  |
  `-----------------*/
  (*myOut) << "INTERFACE\n";
  if(!pInputs.empty())
  {
    (*myOut) << "  INPUT\n    " << *pInputs.begin();
    for(set<string>::iterator it = ++(pInputs.begin());
         it != pInputs.end(); ++it)
    {
      (*myOut) << ", " << *it;
    }
    (*myOut) << ";\n";
  }
  if(!pOutputs.empty())
  {
    (*myOut) << "  OUTPUT\n    " << *pOutputs.begin();
    for(set<string>::iterator it = ++(pOutputs.begin());
         it != pOutputs.end(); ++it)
    {
      (*myOut) << ", " << *it;
    }
    (*myOut) << ";\n";
  }
  if(!pSynchronous.empty())
  {
    (*myOut) << "  SYNCHRONOUS\n    " << *pSynchronous.begin();
    for(set<string>::iterator it = ++(pSynchronous.begin());
         it != pSynchronous.end(); ++it)
    {
      (*myOut) << ", " << *it;
    }
    (*myOut) << ";\n";
  }
  
  (*myOut) << "\nNODES\n";
  for(unsigned int i = 1; i <= stateID_; ++i)
  {
    (*myOut) << "  " << i << " : " << pFormulae[i] << "\n";
    for(map<string, unsigned int>::iterator succ = pSucc[i].begin();
         succ != pSucc[i].end(); ++succ)
    {
      (*myOut) << "    " << succ->first << " -> " << succ->second << "\n";
    }
  }
  
  (*myOut) << endl << std::flush;

  exit(EXIT_SUCCESS); // finished parsing
}
