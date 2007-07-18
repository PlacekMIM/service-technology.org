%{
// options for Bison
#define YYDEBUG 1
#define YYERROR_VERBOSE 0  // for verbose error messages


// to avoid the message "parser stack overflow"
#define YYMAXDEPTH 1000000
#define YYINITDEPTH 10000


#include <string>
#include <iostream>
#include <set>
#include <map>
using std::cerr;
using std::endl;
using std::string;
using std::set;
using std::map;

extern char *stg_yytext;

extern set<string> transitions;
extern set<string> places;
extern set<string> initialMarked;
extern map<string, set<string> > arcs;

set<string> tempNodeSet;
bool in_marking_list = false;
bool in_arc_list = false;


int stg_yyerror(const char* msg)
{
  cerr << msg << ": token last read: `" << stg_yytext << "'" << endl;
  exit(1);
}

// from flex
extern char* stg_yytext;
extern int stg_yylex();
%}

// Bison options
%name-prefix="stg_yy"

%token PLACENAME TRANSITIONNAME IDENTIFIER
%token K_MODEL K_DUMMY K_GRAPH K_MARKING K_END NEWLINE
%token OPENBRACE CLOSEBRACE

%token_table

%union {
  const char *str;
}

/* the types of the non-terminal symbols */
%type <str> TRANSITIONNAME
%type <str> PLACENAME


%%

stg:
  K_MODEL IDENTIFIER NEWLINE NEWLINE
  K_DUMMY transition_list NEWLINE NEWLINE
  K_GRAPH NEWLINE NEWLINE { in_arc_list = true; }
  tp_list pt_list
  K_MARKING { in_marking_list = true; } OPENBRACE place_list CLOSEBRACE NEWLINE NEWLINE
  K_END NEWLINE NEWLINE
;

transition_list:
  TRANSITIONNAME transition_list
    { transitions.insert(string($1));
      if (in_arc_list)
        tempNodeSet.insert(string($1));
    }
| /* empty */
;

place_list:
  PLACENAME place_list
    { places.insert(string($1));
      if (in_marking_list)
        initialMarked.insert(string($1));
      else
        tempNodeSet.insert(string($1));
    }
| /* empty */
;

tp_list:
  TRANSITIONNAME place_list NEWLINE NEWLINE { arcs[string($1)] = tempNodeSet; tempNodeSet.clear(); } tp_list
| /* empty */
;

pt_list:
  PLACENAME transition_list NEWLINE NEWLINE { arcs[string($1)] = tempNodeSet; tempNodeSet.clear(); } pt_list
| /* empty */
;
