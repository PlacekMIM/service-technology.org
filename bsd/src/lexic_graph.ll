/*****************************************************************************\
 BSD -- generating BSD automata

 Copyright (c) 2013 Simon Heiden

 BSD is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 BSD is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with BSD.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/


%option noyywrap
%option nounput
%option full
%option outfile="lex.yy.c"
%option prefix="graph_"

%{
#include <cstring>
#include "syntax_graph.hh"
#include "verbose.h"

void graph_error(const char*);
%}

name      [^,;:()\t \n\{\}][^,;:()\t \n\{\}]*
number    [0-9][0-9]*

%%

"Formula with\n"{number}" subformula(s)." { /* skip */ }

"STATE"      { return KW_STATE; }
"BAD:"      { return KW_BAD; }
"Lowlink:"   { return KW_LOWLINK; }
"SCC:"       { return KW_SCC; }
":"          { return COLON; }
","          { return COMMA; }
"->"         { return ARROW; }

{number}     { graph_lval.val = atoi(graph_text); return NUMBER; }
{name}       { graph_lval.str = strdup(graph_text); return NAME; }

[ \t\r\n]*   { /* skip */ }

%%

__attribute__((noreturn)) void graph_error(const char* msg) {
  status("error near '%s': %s", graph_text, msg);
  abort(9, "error while parsing the reachability graph");
}
