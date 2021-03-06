/*****************************************************************************\
 UML2oWFN -- Translating UML2 Activity Diagrams to Petri nets

 Copyright (C) 2005, 2006, 2007  Niels Lohmann, Christian Gierds,
                                 Martin Znamirowski, and Dirk Fahland

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

/*!
 * \file    globals.h
 *
 * \brief   global variables
 *
 * \author  Niels Lohmann <nlohmann@informatik.hu-berlin.de>,
 *          last changes of: \$Author: nielslohmann $
 *
 * \since   2007/03/05
 *
 * \date    \$Date: 2007/06/28 07:38:16 $
 *
 * \note    This file was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 * \version \$Revision: 1.34 $
 */

#ifndef GLOBALS_H
#define GLOBALS_H





/******************************************************************************
 * Headers
 *****************************************************************************/

#include <set>
#include <string>
#include <map>

#include "options.h"
#include "AST-public.h"   // (for ASTNode)

using std::set;
using std::string;
using std::map;


/*!
 * \brief namespace of all global variables
 */
namespace globals
{

  extern string finalCondition;

  extern ASTNode* rootNode;

  /// string holding the called program name of BPEL2oWFN
  extern string program_name;

  extern string invocation;


  /// working directory of input file
  extern string workingDirectory;

  /// filename of input file
  extern string filename;

  /// directory of output file
  extern string output_directory;

  /// filename of output file
  extern string output_filename;

  /// suffix for output file name, used if multiple files are written
  extern string output_filename_suffix;

  /// flag whether output file name is generated from the given input file
  extern bool getOutputFileNameFromInput;


  /// parameters (set by #parse_command_line)
  extern map<possibleParameters, bool> parameters;

  /// analysis task flags (set by #parse_command_line)
  extern analysis_t analysis;

  /// characteristics of processes to be filtered
  extern map<processCharacteristics, bool> filterCharacteristics;

  /// the last token that was displayed in an error message
  extern string last_error_token;

  /// the line of the last syntax error
  extern string last_error_line;

  extern unsigned int reduction_level;

  /// name of the current process
  extern string currentProcessName;

  /// set of roles to be kept during a rolecut
  extern set<string> keepRoles;

  /// set of exact swim-lane declarations that shall be kept
  extern set< set<string> >  keepRolesExact;

  /// set of Exclusive roles to be matched during a rolecut
  extern set<string> exclusiveRoles;

  /// automatically derive the set of roles to be kept
  extern bool deriveRolesToCut;
}

#endif
