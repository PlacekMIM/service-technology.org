/*****************************************************************************\
 UML2oWFN -- Translating UML2 Activity Diagrams to Petri nets

 Copyright (C) 2007, 2008, 2009  Dirk Fahland and Martin Znamirowski

 UML2oWFN is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 UML2oWFN is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with UML2oWFN.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

/******************************************************************************
 * Headers
 *****************************************************************************/


#include <config.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "uml2owfn-io.h"  // file input and output operations

// include PN Api headers
#include "pnapi/pnapi.h"
#include "petrinet-workflow.h"

#include "debug.h"		  // debugging help
#include "verbose.h"
#include "options.h"
#include "globals.h"
#include "helpers.h"
#include "internal-representation.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::map;


/******************************************************************************
 * External functions
 *****************************************************************************/
extern int frontend_parse();		// from Bison
extern int frontend_debug;			// from Bison
extern int frontend_nerrs;			// from Bison
extern int frontend_lineno;			// from Bison
extern int frontend__flex_debug;		// from flex
extern FILE *frontend_in;			// from flex
extern void frontend_restart(FILE*);		// from flex




/******************************************************************************
 * Global variables
 *****************************************************************************/



/******************************************************************************
 * program parts
 *****************************************************************************/


// analyzation of the commandline
void analyze_cl(int argc, char *argv[])
{

    // setting globals
    globals::program_name = string(argv[0]);

    for (int i = 0; i < argc; i++)
    {
        globals::invocation += string(argv[i]);
        if (i != (argc-1))
            globals::invocation += " ";
    }

    // Reading command line arguments and triggering appropriate behaviour.
    // In case of false parameters call command line help function and exit.
    parse_command_line(argc, argv);

}

/******************************************************************************
 * process translation and model generation methods
 *****************************************************************************/

// translation result flags for handling errors during the translation

#define RES_OK                0
#define RES_EMPTY_PROCESS     1
#define RES_NOT_FREECHOICE    2
#define RES_NO_WF_STRUCTURE   4
#define RES_SKIPPED           8
#define RES_UNCONNECTED       16
#define RES_INSUFF_INTERFACE  32
#define RES_INCORR_STRUCTURE  64
#define RES_NOT_PRESERVED     128 ///< did not preserve soundness

/*!
 * \brief translate the process for soundness analysis according to the given
 *      analysis flags, calls #write_net_file(), #write_task_file(), #extend_script_file()
 *      accordingly
 */
int translate_process_soundness(pnapi::ExtendedWorkflowNet *PN, Process *process, analysis_t analysis, unsigned int reduction_level) {

  int res = RES_OK;

  // ----------------------------------------------------------------
  //  step 1) check given structure of the net (for statistics)
  // ----------------------------------------------------------------

  // check net structure: free choice?
  trace(TRACE_DEBUG, "-> checking whether net is free-choice\n");
  set<pnapi::Node *> nonFC = PN->isFreeChoice();

  // see if the net contains nodes that violate the free-choice property
  if (!nonFC.empty()) {
    res |= RES_NOT_FREECHOICE;
    string witnessString = "";
    for (set<pnapi::Node*>::iterator n=nonFC.begin(); n!=nonFC.end(); n++) {
      witnessString += " '"+(*n)->getName()+"'";
    }
    trace(TRACE_WARNINGS, " [INFO] process is not free-choice: transitions"+witnessString+" are not free-choice.\n");
  }

  // ----------------------------------------------------------------
  //  step 2) extend net to allow soundness analysis, depending on
  //          the termination semantics
  // ----------------------------------------------------------------

#ifdef DEBUG
  trace(TRACE_DEBUG, "-> soundness analysis: creating alpha places\n");
#endif
  // create proper initial marking
  PN->soundness_initialPlaces();

  // set termination semantics that will be used for the analysis
  terminationSemantics_t termination;
  if (analysis[A_TERM_WF_NET]) termination = TERM_ORJOIN;
  else if (analysis[A_TERM_IGNORE_DATA]) termination = TERM_IGNORE_DATA;
  else if (analysis[A_TERM_ORJOIN]) termination = TERM_ORJOIN;
  else termination = TERM_UML_STANDARD;

  // add livelocks only if not creating a workflow net
  bool liveLocks = (termination == TERM_WORKFLOW) ? false : true;
  if (analysis[A_DEADLOCKS]) {
    // create proper livelocks on the net to check for deadlocks
#ifdef DEBUG
    trace(TRACE_DEBUG, "-> soundness analysis: creating omega places for deadlock analysis\n");
#endif
    PN->soundness_terminalPlaces(liveLocks, analysis[A_STOP_NODES], termination);
  }
  else
  {
#ifdef DEBUG
    trace(TRACE_DEBUG, "-> soundness analysis: creating omega places for general soundness analysis\n");
#endif
    PN->soundness_terminalPlaces(liveLocks, analysis[A_STOP_NODES], termination);
  }

  // we want to analyze workflow nets: create a completion
  if (analysis[A_TERM_WF_NET]) {
    if (!PN->complete_to_WFnet_DPE())
      res |= RES_NOT_PRESERVED;
  }
  // finished extending the Petri net


  // ----------------------------------------------------------------
  //  step 3) check extended net for errors
  // ----------------------------------------------------------------
  // alpha to omega?
#ifdef DEBUG
    trace(TRACE_DEBUG, "-> soundness analysis: checking whether each node is on a path from alpha to omega\n");
#endif
  pnapi::Node* ll_node = NULL;
  ll_node = PN->isPathCovered();
  if (ll_node != NULL){
    res |= RES_NO_WF_STRUCTURE;
    trace(TRACE_WARNINGS, " [INFO] process has no workflow-structure: node " + ll_node->getName() + " is not on alpha-omega path\n.");
  }

  if (res != RES_OK) {
    // the net either is not a free-choice net or has no WF-structure,
    // cannot use deadlock analysis to decide soundness
    analysis[A_DEADLOCKS] = false;
  }

  // ----------------------------------------------------------------
  //  step 4) structurally reduce the resulting Petri net
  // ----------------------------------------------------------------
  if (reduction_level > 0) {
    trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
    PN->reduce(globals::reduction_level);

    // check whether net reduction created an empty net
    if (PN->getInternalPlaces().size() == 0) {
      cerr << process->getName() << " retrying (reason: empty process)." << endl;
      cerr << pnapi::io::stat << PN << endl;
      return RES_SKIPPED|RES_EMPTY_PROCESS;
    }
  }


  // ----------------------------------------------------------------
  //  step 5) generate temporal logic properties for checking
  //          soundness, depending on analysis
  // ----------------------------------------------------------------

  if (analysis[A_SOUNDNESS] && !analysis[A_DEADLOCKS]) {
    // generate formula that specifies the final state
    /*
    if (analysis[A_STOP_NODES]) {
      finalStateFormula = PN.createFinalStatePredicate();
    } else*/ {
#ifdef DEBUG
      trace(TRACE_DEBUG, "-> soundness analysis: creating final state formula\n");
#endif
      finalStateFormula = PN->createOmegaPredicate(false);
    }
  }

  if (analysis[A_SAFE]) {
#ifdef DEBUG
      trace(TRACE_DEBUG, "-> safeness analysis: creating safeness formula\n");
#endif
    safeStateFormula = PN->createSafeStatePredicate(/* can be empty? */ globals::parameters[P_TASKFILE]);
    if (safeStateFormula == NULL || safeStateFormula->size() == 0)
      // no formula needed to check safeness, net is safe
      // this field is evaluated when writing the task and script files
      analysis[A_SAFE] = false;
  }

  // ----------------------------------------------------------------
  //  step 6) write Petri net and temporal logic properties to
  //          corresponding files and generate a script file entry
  // ----------------------------------------------------------------

  cerr << process->getName() << "." << endl;

  // extend output file name suffix if necessary

  // the local reduction level is less than the global reduction level set
  // at the commandline
  if (reduction_level < globals::reduction_level) {
    // if we work with the unreduced net, extend the file name
    globals::output_filename_suffix += ".unreduced";
  }

  // generate the output for the given process
  globals::currentProcessName = process->getName();  // for comments about origin in generated net files
  write_net_file(PN, analysis); // write out current process and further related files
  write_task_file(analysis);    // write task file for this process (if applciable)
  extend_script_file(analysis); // extend script file for this process (if applicable)

  if (finalStateFormula != NULL) {
    delete finalStateFormula; // TODO clean up subformulas!
    finalStateFormula = NULL;
  }
  if (safeStateFormula != NULL) {
    delete safeStateFormula;  // TODO clean up subformulas!
    safeStateFormula = NULL;
  }

  return res;
}

/*!
 * \brief decompose Petri net into its swimlanes
 */
void decompose_swimlanes(pnapi::ExtendedWorkflowNet *PN, int *res, std::list< pnapi::ExtendedWorkflowNet > *nets) {

  // collect all swimlanes
  std::set< std::set<string> > swimlanes;
  for (std::set<pnapi::Transition*>::iterator t = PN->getTransitions().begin(); t != PN->getTransitions().end(); t++) {
    swimlanes.insert((*t)->getRoles());
  }

  // for each swimlane
  for (std::set< std::set<string> >::iterator roles = swimlanes.begin(); roles != swimlanes.end(); roles++) {
    /*
    cerr << "---lane---" << endl;
    for (std::set<string>::iterator role = (*roles).begin(); role != (*roles).end(); role++) {
      cerr << *role << " ";
    }
    cerr << endl;
    */

    // create a new Petri net
    pnapi::ExtendedWorkflowNet PN_role;
    PN_role.addRoles((*roles).begin(), (*roles).end());

    // and copy all transitions of this swimlane with pre- and post-places to the new net
    for (std::set<pnapi::Transition*>::iterator t = PN->getTransitions().begin(); t != PN->getTransitions().end(); t++) {
      if (*roles == (*t)->getRoles()) {
        // copy only transitions which are exactly in the swimlane
        pnapi::Transition& tr = PN_role.createTransition((*t)->getName());

        // copy the transition's pre-set
        for (std::set<pnapi::Node*>::iterator p = (*t)->getPreset().begin(); p != (*t)->getPreset().end(); p++) {
          pnapi::Place *pl = PN_role.findPlace((*p)->getName());
          if (pl == NULL) pl = &PN_role.createPlace((*p)->getName());
          PN_role.createArc(*pl, tr);
        }

        // copy the transition's post-set
        for (std::set<pnapi::Node*>::iterator p = (*t)->getPostset().begin(); p != (*t)->getPostset().end(); p++) {
          pnapi::Place *pl = PN_role.findPlace((*p)->getName());
          if (pl == NULL) pl = &PN_role.createPlace((*p)->getName());
          PN_role.createArc(tr, *pl);
        }
      }
    }

    // and save the net
    nets->push_back(PN_role);
  }
}

/*!
 * \brief translate the process for decomposition according to the decomposition
 *      parameters, calls #write_net_file() accordingly
 */
int translate_process_decomposition(pnapi::ExtendedWorkflowNet *PN, Process *process, analysis_t analysis, unsigned int reduction_level) {

    int res = RES_OK;

#ifdef DEBUG
    trace(TRACE_DEBUG, "  cut net\n");
#endif

    std::list< pnapi::ExtendedWorkflowNet > nets;

    if (decomposeInto == CUT_SWIMLANES) {
      decompose_swimlanes(PN, &res, &nets);

    } else if (decomposeInto == CUT_SERVICES) {
      //PN->cutByRoles();
      trace(TRACE_ALWAYS, "Decomposition into services has been disabled in this version.\n");
      res |= RES_SKIPPED;

    } else if (decomposeInto == CUT_SCENARIOS) {
      // TODO: to implement, or separate tool?
      trace(TRACE_ALWAYS, "Decomposition into scenarios has been disabled in this version.\n");
      res |= RES_SKIPPED;

    } else {
      res |= RES_SKIPPED;
      return res;
    }

    // write each decomposed Petri net in a separate file
    int netNum = 0;
    for (std::list< pnapi::ExtendedWorkflowNet >::iterator net = nets.begin(); net != nets.end(); net++) {

      pnapi::ExtendedWorkflowNet roleNet = static_cast<pnapi::ExtendedWorkflowNet>(*net);

      netNum++;

      if (decomposeInto == CUT_SERVICES)
      {
        if (!roleNet.isStructurallyCorrect()) {
          cerr << "structurally incorrect" << endl;
          continue;
          //res |= RES_SKIPPED|RES_INCORR_STRUCTURE;
          //return res;   // do not translate in this case
        }

        // we want to automatically find interesting services from
        // role information in choreographies
        if ( globals::filterCharacteristics[PC_TRIVIAL_INTERFACE]
             && ((roleNet.inputSize() <= 1 && roleNet.outputSize() <= 1)
                 || roleNet.inputSize() == 0
                 || roleNet.outputSize() == 0))
        {
          // but the current process has no interesting interface, and we are
          // filtering
          cerr << "trivial interface" << endl;
          continue;
          //res |= RES_SKIPPED|RES_INSUFF_INTERFACE;
          //return res;
        }

#ifdef DEBUG
        trace(TRACE_DEBUG, "-> decomposition: checking whether net is connected\n");
#endif
        if (!roleNet.isConnected()) {
          cerr << "not connected" << endl;
          continue;
          //res |= RES_SKIPPED|RES_UNCONNECTED;
          //trace(TRACE_WARNINGS, " [INFO] process is not connected.\n\n");
          //return res;   // do not translate in this case
        }
      }


      // perform structural reduction
      if (reduction_level > 0) {
        trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
        roleNet.reduce(globals::reduction_level);

        // check whether net reduction created an empty net
        if ((*net).getInternalPlaces().size() == 0) {
          continue;
          //cerr << process->getName() << " retrying (reason: empty process)." << endl;
          //cerr << pnapi::io::stat << PN << endl;
          //return RES_SKIPPED|RES_EMPTY_PROCESS;
        }
      }

      cerr << process->getName() << "." << toString(netNum) << endl;

      // set output file name by setting file name suffix for this net

      // we have to reset the suffix in every iteration because we write
      // several files with different suffixes, so re-obtain the process file name
      string processName = "";
      // set the current Process name and filename
      if (Process* parent_process=dynamic_cast<Process*>(process->getParent())) {
        processName = parent_process->getName() + "##" + process->getName();
      } else {
        processName = process->getName();
      }

      // set suffix for this process, so we have an output file name
      globals::output_filename_suffix =  "." + process_name_to_file_name(processName);
      globals::output_filename_suffix += "."+toString(netNum);

      // generate the output for the given process
      globals::currentProcessName = process->getName();  // for comments about origin in generated net files
      write_net_file(&roleNet, analysis); // write out current process and further related files

    }

    return res;
}

/*!
 * \brief translate the process for decomposition according to the decomposition
 *      parameters, calls #write_net_file() accordingly
 */
int translate_process_normal(pnapi::ExtendedWorkflowNet *PN, Process *process, analysis_t analysis, unsigned int reduction_level) {

  int res = RES_OK;

  // ----------------------------------------------------------------
  //  step 1) structurally reduce the resulting Petri net
  // ----------------------------------------------------------------
  if (reduction_level > 0) {
    trace(TRACE_INFORMATION, "-> Structurally simplifying Petri Net ...\n");
    PN->reduce(globals::reduction_level);

    // check whether net reduction created an empty net
    if (PN->getInternalPlaces().size() == 0) {
      cerr << process->getName() << " retrying (reason: empty process)." << endl;
      cerr << pnapi::io::stat << PN << endl;
      return RES_SKIPPED|RES_EMPTY_PROCESS;
    }
  }

  // ----------------------------------------------------------------
  //  step 2) write Petri net to corresponding files
  // ----------------------------------------------------------------
  cerr << process->getName() << "." << endl;

  // extend output file name suffix if necessary

  // the local reduction level is less than the global reduction level set
  // at the commandline
  if (reduction_level < globals::reduction_level) {
    // if we work with the unreduced net, extend the file name
    globals::output_filename_suffix += ".unreduced";
  }

  // generate the output for the given process
  globals::currentProcessName = process->getName();  // for comments about origin in generated net files
  write_net_file(PN, analysis); // write out current process and further related files

  return res;
}


/*!
 * \brief translate the process according to the given analysis flags,
 *      calls #write_net_file(), #write_task_file(), #extend_script_file()
 *      accordingly
 */
int translate_process(Process *process, analysis_t analysis, unsigned int reduction_level)
{
  int res = RES_OK;

  trace(TRACE_WARNINGS, "\n");
  trace(TRACE_WARNINGS, "generating Petri net for " + process->getName()+ "\n");

  // Generates a petri net from the current process
  pnapi::ExtendedWorkflowNet *PN = new pnapi::ExtendedWorkflowNet();   // get a new net
  process->translateToNet(PN);
#ifdef DEBUG
  trace(TRACE_DEBUG, "  -> done\n");
  trace(TRACE_DEBUG, "  modify net\n");
#endif

  // Fix some errors in the net due to invalid input models.
  // Delete places in the net that represent unused pins
  // of tasks in the business object model
  if (!globals::parameters[P_KEEP_UNCONN_PINS]) {
#ifdef DEBUG
    trace(TRACE_DEBUG, "-> removing unconnected pins\n");
#endif
    PN->removeUnconnectedPins();
    //PN.removeEmptyOutputPinSets();
  }

  // if we ware to cut the process by its roles, we now cut the petrinet
  // according to global parameters
  if (options[O_ROLECUT]) {
  }

  if (analysis[A_SOUNDNESS]) {
    // extend net for soundness checking and write net to file,
    // generate analysis tasks if applicable
    res |= translate_process_soundness(PN, process, analysis, reduction_level);
  } else if (options[O_ROLECUT]) {
    // decompose net and write to net file(s)
    res |= translate_process_decomposition(PN, process, analysis, reduction_level);
  } else {
    // just write the translated net to files
    res |= translate_process_normal(PN, process, analysis, reduction_level);
  }

  // TODO: WARNING: the allocated net is not freed to avoid
  //                a bug with the GCC under Linux and Cygwin
  //  delete PN;    // finish Petri net
  //  PN = NULL;

  return res;
}

int translate_process_reconcile (Process *process, analysis_t analysis)
{
  int result = translate_process(process, analysis, globals::reduction_level);

  if (result & RES_EMPTY_PROCESS) // got empty net, try without reduction
    return translate_process(process, analysis, 0 /* no reduction */);

  return result;
}



/******************************************************************************
 * main() function
 *****************************************************************************/

/*!
 * \brief entry point of UML2OWFN
 *
 * Controls the behaviour of input and output.
 *
 * \param argc	number of command line arguments
 * \param argv	array with command line arguments
 *
 * \returns 0 if everything went well
 * \returns 1 if an error occurred
 *
 * \todo The opening and closing of files is awkward.
 *
 * \bug  Piping does not always work...
 */
int main( int argc, char *argv[])
{


    // analyzation of the commandline
    analyze_cl(argc,argv);

    // parsing all inputfiles
    for (set< string >::const_iterator file = inputfiles.begin(); file != inputfiles.end(); file++)
    {
        // open the current file
        open_file(*file);

        // initialize the AST root node
        globals::rootNode = new ASTNode("uml2owfn_XML_root", NULL);

        // reset the parser
        frontend_restart(frontend_in);

        // set output file name (if necessary)
        if (globals::getOutputFileNameFromInput) {
            globals::output_filename = stripExtension(globals::filename);
        }

        // invoke XML parser
        trace(TRACE_INFORMATION, "Parsing " + globals::filename + " ...\n");
        int parse_result = frontend_parse();
        if (parse_result)
          abort(3, "Failed to parse %s\n", globals::filename.c_str());
        trace(TRACE_INFORMATION, "Parsing of " + globals::filename + " complete.\n");

        // create lists for the global processes, tasks and services so
        // that callTo-tasks can instantiate them
        list<Process*> dummyProcesses;
        list<SimpleTask*> dummyTasks;
        list<SimpleTask*> dummyServices;

        // create a list containing all processes that are meant for translation
        list<Process*> allProcesses;

        // detailed output of the parsed xml tree for debugging
        if (debug_level >= TRACE_VERY_DEBUG) {
            globals::rootNode->debugOutput(" ");
        }


        trace(TRACE_INFORMATION, "Parsing XML-tree...\n");

        // process all xml elements gathering information about the global processes, tasks and services
        globals::rootNode->prepareInstances(dummyProcesses,dummyTasks,dummyServices);

        // completly translate the xml tree into the internal representation of the BOM model
        globals::rootNode->finishInternal(dummyProcesses,dummyTasks,dummyServices,allProcesses);

        trace(TRACE_INFORMATION, "Parsing of XML-tree complete.\n");


        // anonymize all process names
        if (globals::parameters[P_ANONYMIZE]) {
          int processNum = 0;           // running number of processes
          for(list<Process*>::iterator currProcess = allProcesses.begin(); currProcess != allProcesses.end(); currProcess++) {
            Process *process = *currProcess;
            processNum++;
            process->setName("net"+toString(processNum));
          }
        }

        log_print("NET");
        log_print(";COMPLEX SYNTAX (UML);SYNTAX FEATURES (UML);NUM NODES (UML);NUM EDGES (UML);AVG INDEGREE (UML); AVG OUTDEGREE (UML);MAX INDEGREE (UML); MAX OUTDEGREE (UML)");
        log_println(";NET PROPERTIES (PN)");

        int processNum = 0;           // running number of processes
        int translatedProcesses = 0;  // translated number of processes
        // number of processes where workflow completion does not preserve soundness
        int soundViolatingWfCompletion = 0;
        for(list<Process*>::iterator currProcess = allProcesses.begin(); currProcess != allProcesses.end(); currProcess++) {
          Process *process = *currProcess;
          processNum++;

          log_print(process->getName());

          process->updateCharacteristics();
          if (process->empty()) {
              process->processCharacteristics |= UML_PC(PC_EMPTY);
          }

          UmlProcessStatistics stat = process->getStatistics();
          log_print(stat.toString(';'));

          // check if this process has to be filtered
          if (process->processCharacteristics != UML_PC(PC_NORMAL)) {

            string reason = "";
            if (UML_PC(PC_OVERLAPPING) & process->processCharacteristics && globals::filterCharacteristics[PC_OVERLAPPING])
              reason += " overlapping pinsets,";
            if (UML_PC(PC_PIN_MULTI) & process->processCharacteristics && globals::filterCharacteristics[PC_PIN_MULTI])
              reason += " pin multiplicities,";
            if (UML_PC(PC_PIN_MULTI_NONMATCH) & process->processCharacteristics && globals::filterCharacteristics[PC_PIN_MULTI_NONMATCH])
              reason += " non-matching pin multiplicities,";
            if (UML_PC(PC_EMPTY) & process->processCharacteristics && globals::filterCharacteristics[PC_EMPTY])
              reason += " empty process,";

            if (reason != "") {
              // at least one reason filter has been set, skip this process
              trace(TRACE_INFORMATION, "filtering process "+process->getName()+", reason: "+reason+"\n");
              log_println(";---");  // fill last columns of log
              continue;
            }
          }

          // translate the process to a petrinet and generate the output
          // if the process is to be cut by its roles later on, we need to spread the roles to role-less flow content elements
          if (options[O_ROLECUT]) {
#ifdef DEBUG
            trace(TRACE_DEBUG, "  spread roles\n");
#endif
            process->spreadRoles();

#ifdef DEBUG
            trace(TRACE_DEBUG, "keeping the following roles:\n");
            for (set<string>::const_iterator r = globals::keepRoles.begin(); r != globals::keepRoles.end(); r++)
              trace(TRACE_DEBUG, "  "+(*r)+"\n");
#endif
          }

          // if debug mode is high enough print a debug output for this process
          if (debug_level >= TRACE_VERY_DEBUG) {
              process->debugOutput("  ");
          }

          string processName = "";
          // set the current Process name and filename
          if (Process* parent_process=dynamic_cast<Process*>(process->getParent())) {
            processName = parent_process->getName() + "##" + process->getName();
          } else {
            processName = process->getName();
          }

          // set suffix for this process, so we have an output file name
          globals::output_filename_suffix =  "." + process_name_to_file_name(processName);

          //process->debugOutput("  ");
          /*
          if (options[O_ROLECUT] && globals::keepRoles.empty()) {
              trace(TRACE_INFORMATION, "Process: " + process->getName() + " did not contain any roles and thus will not be cut.\n");
          }
          */


          // translate process wrt. the current analysis settings, if necessary,
          // multiple translations and analyses for the same processes can be
          // invoked, just add another call to translate_process_reconcile
          // with different analysis settings
          int res = translate_process_reconcile(process, globals::analysis);

          // evaluate translation result for this process
          if (res & RES_EMPTY_PROCESS)
            log_print(";empty net");
          else {
            if (res & RES_SKIPPED) {
              log_print(";");
              // process was skipped
              if (res & RES_UNCONNECTED) log_print("not connected/");
              if (res & RES_INSUFF_INTERFACE) log_print("insufficient interface/");
              if (res & RES_INCORR_STRUCTURE) log_print("net is no longer a UML2-AD/");
            } else {
              // process was translated
              translatedProcesses++;
              // no structural specifics
              if (res == RES_OK) log_print(";---");
              else {
                // structural specifics
                log_print(";");
                if (res & RES_NOT_FREECHOICE) log_print("not free-choice/");
                if (res & RES_NO_WF_STRUCTURE) log_print("no workflow structure/");
                if (res & RES_NOT_PRESERVED) {
                  log_print("did not preserve soundness/");
                  soundViolatingWfCompletion++;
                }
              }
            }
          }

          log_println("");

          if (globals::deriveRolesToCut) {
            globals::exclusiveRoles.clear();
            globals::keepRoles.clear();
          }
      } // for all processes of the current file

      if (translatedProcesses > 1)
        message("%i of %i processes have been translated.", translatedProcesses, processNum);
      else if (translatedProcesses == 1)
        message("%i of %i processes has been translated.", translatedProcesses, processNum);
      else
        message("None of %i processes have been translated.", processNum);

      if (soundViolatingWfCompletion >= 1)
        message("For %i of %i processes soundness could not be preserved.\n", soundViolatingWfCompletion, translatedProcesses);
      else
        message("All processes have been translated preserving soundness.");


      write_script_file();  // write script file for the entire process library
      write_log_file();     // write translation log for the entire library

      close_file(*file);
    } // for all input files

    return EXIT_SUCCESS;
}

/*!
 * \defgroup frontend Front End
 */
