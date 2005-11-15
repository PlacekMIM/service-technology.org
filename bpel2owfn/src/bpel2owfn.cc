/*!
 * \file main.cc
 *
 * \brief The control component for BPEL2oWFN
 *
 * This file controls the behaviour of BPEL2oWFN and is the interface to the environment.
 * 
 * \author  
 *          - responsible: Christian Gierds <gierds@informatik.hu-berlin.de>
 *          - last changes of: \$Author: gierds $
 *          
 * \date
 *          - created: 2005/10/18
 *          - last changed: \$Date: 2005/11/15 13:11:50 $
 * 
 * \note    This file is part of the tool BPEL2oWFN and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/forschung/projekte/tools4bpel
 *          for details.
 *
 * \version \$Revision: 1.21 $
 *          - 2005-11-15 (gierds) Moved command line evaluation to helpers.cc.
 *
 */

#include "main.h"

/// The Petri Net
PetriNet *TheNet = new PetriNet();

// some file names and pointers

/// Filename of input file.
std::string filename = "";
/// Filename of dot output file
std::string dot_filename = "";
/// Pointer to dot output file
std::ostream * dot_output = &std::cout;

// different modes controlled by command line

/// print the Petri Net
bool mode_petri_net = false;
/// simplify Petri Net
bool mode_simplify_petri_net = false;
/// paint Petri Net with dot
bool mode_dot_petri_net = false;
/// paint Petri Net with dot and output to file
bool mode_dot_2_file = false;
/// pretty printer
bool mode_pretty_printer = false;
/// print AST
bool mode_ast = false;
/// print the "low level" Petri Net
bool mode_low_level_petri_net = false;


/**
 * Entry point for BPEL2oWFN.
 * Controls the behaviour of input and output.
 *
 * \parameter argc	number of command line arguments
 * \parameter argv	array with command line arguments
 *
 * \returns Error code (0 if everything went well)
 *
 * \todo
 * 	- integrate low level Petri Net option
 * 
 */
int main( int argc, char *argv[])
{
  /***
   * Reading command line arguments and triggering appropriate behaviour.
   * In case of false parameters call command line help function and exit.
   */
  parse_command_line(argc, argv);
	
  trace(TRACE_INFORMATION, "Parsing ...\n");
  
  // invoke Bison parser
  int error = yyparse();

  if (!error)
  {
    trace(TRACE_INFORMATION, "Parsing complete.\n");
    if (mode_ast)
    {
      TheProcess->print();
    }
    
    trace(TRACE_INFORMATION, "Rewriting BPEL...\n");
    TheProcess = TheProcess->rewrite(kc::implicit);
    trace(TRACE_INFORMATION, "Rewriting complete.\n");
    
    if (mode_pretty_printer)
    {
      TheProcess->unparse(kc::printer, kc::xml);
    }
    if (mode_petri_net)
    {
      TheProcess->unparse(kc::pseudoPrinter, kc::petrinet);
    }
  }

  if (mode_simplify_petri_net)
  {
    TheNet->simplify();
  }
  if (mode_dot_petri_net)
  {
    TheNet->drawDot();
  }


  
  //std::cout << std::endl;

  //TheNet->information();


  if (dot_filename != "")
  {
    trace(TRACE_INFORMATION,"Closing dot output file: " + dot_filename + "\n");
    (*dot_output) << std::flush;
    ((std::ofstream*)dot_output)->close();
  }
  
  return error;
}
