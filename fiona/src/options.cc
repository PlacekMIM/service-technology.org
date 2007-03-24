/*****************************************************************************
 * Copyright 2005, 2006 Niels Lohmann, Peter Massuthe, Daniela Weinberg,     *
 *                      Jan Bretschneider, Christian Gierds, Dennis Reinert  *
 *                                                                           *
 * This file is part of Fiona.                                               *
 *                                                                           *
 * Fiona is free software; you can redistribute it and/or modify it          *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * Fiona is distributed in the hope that it will be useful, but WITHOUT      *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
 * more details.                                                             *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with Fiona; if not, write to the Free Software Foundation, Inc., 51       *
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.                      *
 *****************************************************************************/

/*!
 * \file    options.cc
 *
 * \brief   options management for fiona
 *
 * \author  responsible: Daniela Weinberg <weinberg@informatik.hu-berlin.de>
 *
 * \note    This file is part of the tool Fiona and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 */

#include "mynew.h"
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <stdlib.h>

#include "options.h"
#include "debug.h"
#include "fiona.h"

using namespace std;

// some file names and pointers
char * netfile;
std::list<char*> netfiles; 
std::string ogfile;

int commDepth_manual;
int events_manual;
unsigned int messages_manual;
int bdd_reordermethod;

/// Filename of input file
std::string filename = "<STDIN>";

/// Filename of input file
std::string output_filename = "";

/// pointer to input stream
std::istream * input = &std::cin;
/// pointer to output stream
std::ostream * output = &std::cout;
/// pointer to log stream
std::ostream * log_output = &std::cout;   // &std::clog;

///// Filename of log file
//std::string log_filename = "";
//
//bool createOutputFile = false;

// different modes controlled by command line

std::map<possibleOptions,    bool> options;
std::map<possibleParameters, bool> parameters;


// values getopt_long() should return for long options that have no short
// version. Start with some value that cannot be the value of a char.
#define GETOPTLONG_MATCH  256

// long options
static struct option longopts[] =
{
  { "help",            no_argument,       NULL, 'h' },
  { "version",         no_argument,       NULL, 'v' },
  { "debug",           required_argument, NULL, 'd' },
  { "net",             required_argument, NULL, 'n' },
  { "graphtype",       required_argument, NULL, 't' },
  { "show",            required_argument, NULL, 's' },
  { "calcallstates",   no_argument,       NULL, 'a' },
  { "reduceIG",        no_argument,       NULL, 'r' },
  { "messagemaximum",  required_argument, NULL, 'm' },
  { "eventsmaximum",   required_argument, NULL, 'e' },
  { "BDD",             required_argument, NULL, 'b' },
  { "OnTheFly",        required_argument, NULL, 'B' },
  { "exchangeability", no_argument,       NULL, 'x' },
  { "match",           required_argument, NULL, GETOPTLONG_MATCH },
  { NULL,              0,                 NULL, 0   }
};


const char * par_string = "hvd:n:t:s:arm:e:b:B:x";

// --------------------- functions for command line evaluation ------------------------
// Prints an overview of all commandline arguments.
void print_help() {
  trace("Options: (if an option is skipped, the default settings are denoted)\n");
  trace("\n");
  trace(" -h | --help ................... print this information and exit\n");
  trace(" -v | --version ................ print version information and exit\n");
  trace(" -d | --debug=<level> .......... set debug <level>:\n");
  trace("                                   1 - show nodes and dfs information\n");
  trace("                                   2 - show analyse information (i.e. colors)\n");
  trace("                                   3 - show information on events and states\n");
  trace("                                   4 - yet to be defined ;)\n");
  trace("                                   5 - show detailed information on everything\n");
  trace(" -n | --net=<filename> ......... read input owfn from <filename>\n");
  trace(" -t | --graphtype=<type> ....... select the graph <type> to be calculated:\n");
  trace("                                   OG - operating guideline\n");
  trace("                                   IG - interaction graph (default)\n");
  trace(" -m | --messagemaximum=<level>   set maximum number of same messages per state\n");
  trace("                                 to <level>\n");
  trace(" -e | --eventsmaximum=<level>    set event to occur at most <level> times\n");
  trace("                                 (default is 1)\n");
  trace("                                 (-1 means disabling -e option -- only possible if -m option is set)\n");
  trace("                                 (only relevant for OG)\n");
  trace(" -r | --reduceIG ............... use reduction rules for IG\n");
  trace(" -s | --show=<parameter> ....... different display options <parameter>:\n");
  trace("                                   allnodes - show nodes of all colors\n");
  trace("                                   blue     - show blue nodes only (default)\n");
  trace("                                   rednodes - show blue and red nodes (no empty\n");
  trace("                                              node and no black nodes)\n");
  trace("                                   empty    - show empty node\n");
  trace("                                   allstates- show all calculated states per\n");
  trace("                                              node\n");
  trace(" -x | --exchangeability ........ check for two oWFN the equality of its\n");
  trace("                                 operating guidelines (the BDD-representation\n");
  trace("                                 must have been computed before)\n");
  trace("                                 syntax: -n netfile1.owfn -n netfile2.owfn -x\n");
  trace(" -b | --BDD=<reordering> ....... enable BDD construction (only relevant for OG)\n");
  trace("                                 argument <reordering> specifies reodering\n");
  trace("                                 method:\n");
  trace("                                    0 - CUDD_REORDER_SAME\n");
  trace("                                    1 - CUDD_REORDER_NONE\n");
  trace("                                    2 - CUDD_REORDER_RANDOM\n");
  trace("                                    3 - CUDD_REORDER_RANDOM_PIVOT\n");
  trace("                                    4 - CUDD_REORDER_SIFT\n");
  trace("                                    5 - CUDD_REORDER_SIFT_CONVERGE\n");
  trace("                                    6 - CUDD_REORDER_SYMM_SIFT\n");
  trace("                                    7 - CUDD_REORDER_SYMM_SIFT_CONV\n");
  trace("                                    8 - CUDD_REORDER_WINDOW2\n");
  trace("                                    9 - CUDD_REORDER_WINDOW3\n");
  trace("                                   10 - CUDD_REORDER_WINDOW4\n");
  trace("                                   11 - CUDD_REORDER_WINDOW2_CONV\n");
  trace("                                   12 - CUDD_REORDER_WINDOW3_CONV\n");
  trace("                                   13 - CUDD_REORDER_WINDOW4_CONV\n");
  trace("                                   14 - CUDD_REORDER_GROUP_SIFT\n");
  trace("                                   15 - CUDD_REORDER_GROUP_SIFT_CONV\n");
  trace("                                   16 - CUDD_REORDER_ANNEALING\n");
  trace("                                   17 - CUDD_REORDER_GENETIC\n");
  trace("                                   18 - CUDD_REORDER_LINEAR\n");
  trace("                                   19 - CUDD_REORDER_LINEAR_CONVERGE\n");
  trace("                                   20 - CUDD_REORDER_LAZY_SIFT\n");
  trace("                                   21 - CUDD_REORDER_EXACT\n");
  trace(" -B | --OnTheFly=<reordering> ...enable BDD construction on the fly\n");
  trace("                                 (only relevant for OG)\n");
  trace("                                 argument <reordering> specifies reodering\n");
  trace("                                 method (see option -b)\n");
  trace(" --match=<OG filename> ......... check if given oWFN (-n) matches with\n");
  trace("                                 operating guideline given in <OG filename>\n");
  trace("\n");
  trace("\n");
  trace("For more information see:\n");
  trace("  http://www.informatik.hu-berlin.de/top/tools4bpel/fiona\n");
  trace("\n");
}

// Prints some version information
void print_version() {
  trace(std::string(PACKAGE_STRING) + " -- ");
  trace("Functional InteractiON Analysis of open workflow nets\n");
  trace("\n");
  trace("Copyright (C) 2005, 2006 Peter Massuthe and Daniela Weinberg\n");
  trace("This is free software; see the source for copying conditions. There is NO\n");
  trace("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

void printErrorInvalidNumberForOptionAndExit(const std::string& option,
    const std::string& givenArgument)
{
    cerr << "Error:\tArgument for option " << option << " has to be a "
            "non-negative number." << endl
         << "\tInstead you gave '" << givenArgument
         << "' as the argument for " << option << '.' << endl
         << "\tEnter \"fiona --help\" for more information."
         << endl;

    exit(1);
}

void testForInvalidArgumentNumberAndPrintErrorAndExitIfNecessary(
    const std::string& option, const std::string& givenArgument)
{
    if (!isNonNegativeInteger(givenArgument)) {
        printErrorInvalidNumberForOptionAndExit(option, optarg);
    }
}

// get parameters from options
void parse_command_line(int argc, char* argv[]) {

    int i;

    // the program name on the commandline
    std::string progname = std::string(argv[0]);

    // initialize options
    options[O_HELP] = false;
    options[O_VERSION] = false;
    options[O_DEBUG] = false;
    options[O_GRAPH_TYPE] = false;
    options[O_SHOW_NODES] = false;
    options[O_CALC_ALL_STATES] = false; // standard: man muss -a angeben, um voll
//    options[O_CALC_ALL_STATES] = true; // so lange Reduktion im Teststadium
    options[O_CALC_REDUCED_IG] = false;
    options[O_OWFN_NAME] = false;
    options[O_BDD] = false;
    options[O_OTF] = false;
    options[O_EX] = false;
    options[O_MATCH] = false;

    options[O_MESSAGES_MAX] = false;
    options[O_EVENT_USE_MAX] = true;


    // initialize parameters
    parameters[P_IG] = true;
    parameters[P_OG] = false;
    parameters[P_SHOW_ALL_NODES] = false;
    parameters[P_SHOW_BLUE_NODES_ONLY] = true;
    parameters[P_SHOW_NO_RED_NODES] = true;
    parameters[P_SHOW_EMPTY_NODE] = false;
    parameters[P_SHOW_STATES_PER_NODE] = false;

    bdd_reordermethod = 0;

    messages_manual = 1;
    events_manual = 1;

    // evaluate options and set parameters
    trace(TRACE_0, "\n");
    int optc = 0;
    while ((optc = getopt_long (argc, argv, par_string, longopts, (int *) 0))
           != EOF)
    {
        switch (optc) {
            case 'h': print_help();      exit(0);
            case 'v': print_version();   exit(0);
            case 'd':
                if (string(optarg) == "1") {
                    options[O_DEBUG] = true;
                    debug_level = TRACE_1;
                } else if (string(optarg) == "2") {
                    options[O_DEBUG] = true;
                    debug_level = TRACE_2;
                } else if (string(optarg) == "3") {
                    options[O_DEBUG] = true;
                    debug_level = TRACE_3;
                } else if (string(optarg) == "4") {
                    options[O_DEBUG] = true;
                    debug_level = TRACE_4;
                } else if (string(optarg) == "5") {
                    options[O_DEBUG] = true;
                    debug_level = TRACE_5;
                } else {
                    cerr << "Error:\twrong debug mode" << endl
                         << "\tEnter \"fiona --help\" for more information."
                         << endl;
                    exit(1);
                }
                break;
            case 'n':
                if (optarg) {
                    options[O_OWFN_NAME] = true;
                    // netfile = optarg;
                    netfiles.push_back(optarg);
                } else {
                    cerr << "Error:\tnet name missing" << endl
                         << "\tEnter \"fiona --help\" for more information."
                         << endl;
                    exit(1);
                }
                break;
            case 't':
                if (string(optarg) == "OG") {
                    options[O_GRAPH_TYPE] = true;
                    parameters[P_OG] = true;
                    parameters[P_IG] = false;
                } else if (string(optarg) == "IG") {
                    options[O_GRAPH_TYPE] = true;
                    parameters[P_OG] = false;
                    parameters[P_IG] = true;
                } else {
                    cerr << "Warning:\twrong graph type" << endl
                         << "\tIG computed" << endl;
                    options[O_GRAPH_TYPE] = true;
                    parameters[P_OG] = false;
                    parameters[P_IG] = true;
                }
                break;
            case 'm':
                options[O_MESSAGES_MAX] = true;
                testForInvalidArgumentNumberAndPrintErrorAndExitIfNecessary(
                    "-m", optarg);
                messages_manual = atoi(optarg);
                break;
            case 'e':
                events_manual = atoi(optarg);
                
                if (events_manual < 0) {
                	options[O_EVENT_USE_MAX] = false;                
                }
                break;
            case 's':
                if (string(optarg) == "blue") {
                    options[O_SHOW_NODES] = true;
                    parameters[P_SHOW_ALL_NODES] = false;
                    parameters[P_SHOW_NO_RED_NODES] = false;
                    parameters[P_SHOW_BLUE_NODES_ONLY] = true;
                } else if (string(optarg) == "rednodes") {
                    options[O_SHOW_NODES] = true;
                    parameters[P_SHOW_ALL_NODES] = false;
                    parameters[P_SHOW_NO_RED_NODES] = false;
                    parameters[P_SHOW_BLUE_NODES_ONLY] = false;
                } else if (string(optarg) == "empty") {
                    options[O_SHOW_NODES] = true;
                    parameters[P_SHOW_EMPTY_NODE] = true;
                } else if (string(optarg) == "allnodes") {
                    options[O_SHOW_NODES] = true;
                    parameters[P_SHOW_ALL_NODES] = true;
                    parameters[P_SHOW_EMPTY_NODE] = true;
                    parameters[P_SHOW_NO_RED_NODES] = false;
                    parameters[P_SHOW_BLUE_NODES_ONLY] = false;
                } else if (string(optarg) == "allstates") {
                    options[O_SHOW_NODES] = true;
                    parameters[P_SHOW_STATES_PER_NODE] = true;
                } else {
                    cerr << "Error:\twrong show option" << endl
                         << "\tEnter \"fiona --help\" for more information."
                         << endl;
                    exit(1);
                }
                break;
            case 'a':
                options[O_CALC_ALL_STATES] = true;
                break;
            case 'r':
                options[O_CALC_REDUCED_IG] = true;
                break;
            case 'b':
                options[O_BDD] = true;
                testForInvalidArgumentNumberAndPrintErrorAndExitIfNecessary(
                    "-b", optarg);
                i = atoi(optarg);
                if (i >= 0 && i <= 21){
                    bdd_reordermethod = i;
                } else {
                    cerr << "Error:\twrong BDD reorder method" << endl
                         << "\tEnter \"fiona --help\" for more information."
                         << endl;
                    exit(1);
                }
                break;
            case 'B':
                options[O_OTF] = true;
                options[O_BDD] = false;
                testForInvalidArgumentNumberAndPrintErrorAndExitIfNecessary(
                    "-B", optarg);
                i = atoi(optarg);
                if (i >= 0 && i <= 21){
                    bdd_reordermethod = i;
                } else {
                    cerr << "Error:\twrong BDD reorder method" << endl
                         << "\tEnter \"fiona --help\" for more information."
                         << endl;
                    exit(1);
                }
                break;
            case 'x':
                options[O_EX] = true;
                options[O_GRAPH_TYPE] = false;
                parameters[P_IG] = false;
                parameters[P_OG] = false;
                break;
            case GETOPTLONG_MATCH:
                if (optarg) {
                    options[O_MATCH] = true;
                    ogfile = optarg;
                } else {
                    cerr << "Error:\tOG file name missing" << endl
                         << "\tEnter \"fiona --help\" for more information."
                         << endl;
                    exit(1);
                }
                break;
            case '?':
                cerr << "Error:\toption error" << endl
                     << "\tEnter \"fiona --help\" for more information."
                     << endl;
                exit(1);
            default:
                cerr << "Warning:\tUnknown option ignored" << endl
                     << "\tEnter \"fiona --help\" for more information"
                     << endl;
                break;
        }
    }

    if (options[O_OWFN_NAME] == false) {
        cerr << "Error:\tmissing parameter -n" << endl
             << "\tEnter \"fiona --help\" for more information." << endl;
        exit(1);
    }

    if (options[O_EVENT_USE_MAX] == false && options[O_MESSAGES_MAX] == false) {
        cerr << "if no limit for using events is given, you must specify a message bound via option -m " << endl
             << "\tEnter \"fiona --help\" for more information.\n" << endl;
        exit(1);
    }

    if (options[O_BDD] && parameters[P_OG] == false && options[O_EX] == false) {
        cerr << "computing IG -- BDD option ignored\n" << endl;
        options[O_BDD] = false;
    }
}


