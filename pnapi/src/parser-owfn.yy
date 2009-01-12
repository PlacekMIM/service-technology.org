/* -*- mode: c++ -*- */

/*!
 * \file    parser-owfn.yy
 *
 * \brief   bison parser for OWFN files
 *
 * \author  Robert Waltemath <robert.waltemath@uni-rostock.de>
 *          last changes of: $Author$
 *
 * \since   2008/11/10
 *
 * \date    $Date$
 *
 * \note    Large parts of the code are copied from the bpel2owfn OWFN parser.
 *
 * \version $Revision$
 */


 /*****************************************************************************
  * bison options 
  ****************************************************************************/

/* plain c parser: the prefix is our "namespace" */
%name-prefix="pnapi_owfn_"

/* write tokens to parser-owfn.h for use by scanner */
%defines


 /*****************************************************************************
  * C declarations
  ****************************************************************************/
%{

#include <string>

#include "util.h"
#include "petrinet.h"
#include "parser.h"

using pnapi::PetriNet;
using pnapi::Place;
using pnapi::Transition;
using namespace pnapi::parser::owfn;

// defined in petrinet-input-owfn-lexer.cc (generated by flex)
extern int yylex();

// global variable for resulting net
Node * pnapi_owfn_ast;

// global variables for use in actions during net construction
PetriNet * PN;
int readmode = 0;
string nodename;
string current_port;
Transition * t = NULL; 
set<string> in;
set<string> out;
set<pair<Place *, unsigned int> > finalMarking;

// standard error function (defined in epilogue)
int yyerror(const char *);

%}


 /*****************************************************************************
  * types, tokens, start symbol
  ****************************************************************************/

%union {
  int yt_int;
  string * yt_string;
  pnapi::parser::owfn::Node * yt_node;
}

%type <yt_string> IDENT
%type <yt_string> nodeident
%type <yt_int> NUMBER
%type <yt_int> NEGATIVE_NUMBER

%type <yt_node> final 
%type <yt_node> statepredicate

%token KEY_SAFE KEY_PLACE KEY_INTERNAL KEY_INPUT KEY_OUTPUT KEY_PORTS
%token KEY_MARKING KEY_FINALMARKING KEY_FINALCONDITION
%token KEY_TRANSITION KEY_CONSUME KEY_PRODUCE
%token KEY_ALL_OTHER_PLACES_EMPTY
%token KEY_ALL_OTHER_INTERNAL_PLACES_EMPTY
%token KEY_ALL_OTHER_EXTERNAL_PLACES_EMPTY
%token KEY_MAX_UNIQUE_EVENTS KEY_ON_LOOP KEY_MAX_OCCURRENCES
%token KEY_TRUE KEY_FALSE LCONTROL RCONTROL
%token COMMA COLON SEMICOLON IDENT NUMBER NEGATIVE_NUMBER
%token LPAR RPAR

%left OP_OR
%left OP_AND
%left OP_NOT
%nonassoc OP_EQ OP_NE OP_GT OP_LT OP_GE OP_LE

%start net


 /*****************************************************************************
  * grammar rules 
  ****************************************************************************/
%%

net:                 { 
                       PN = new PetriNet();
		       readmode = 0;
		       t = NULL; 
		       in.clear();
		       out.clear();
		       finalMarking.clear();
                     }
                     KEY_PLACE place_area port_area KEY_MARKING markinglist 
                     SEMICOLON final transitionlist 
                     { pnapi_owfn_ast = new Node(Node::PETRINET, PN, $8); }
                   ;

final:               KEY_FINALMARKING finalmarkinglist SEMICOLON { $$ = NULL; }
                   | KEY_FINALCONDITION statepredicate SEMICOLON { $$ = $2; }
                   ;

place_area:          place_area_internal place_area_input place_area_output
	           | place_area_lola
                   ;

place_area_input:    { readmode = 0; } KEY_INPUT placelists SEMICOLON
                   | /* empty */
                   ;

place_area_output:   { readmode = 1; } KEY_OUTPUT placelists SEMICOLON
                   | /* empty */
                   ;

place_area_internal: { readmode = 2; } KEY_INTERNAL placelists SEMICOLON
                   | /* empty */
                   ;

place_area_lola:     { readmode = 2; } placelists SEMICOLON
                   ;

placelists:          capacity placelist 
                   | placelists SEMICOLON capacity placelist
                   ;

capacity:            /* empty */
                   | KEY_SAFE COLON  
                   | KEY_SAFE NUMBER COLON 
                   ;

placelist:           placelist COMMA place 
                   | place
                   | /* empty */ 
                   ;

place:               nodeident controlcommands {
                     switch (readmode) {
		     case 0: PN->createPlace(nodename, pnapi::Node::INPUT); break;
		     case 1: PN->createPlace(nodename, pnapi::Node::OUTPUT); break;
		     case 2: PN->createPlace(nodename); break;
		     case 3: break;
		     } }
                   ;     

nodeident:           IDENT  { nodename = *$1; }
                   | NUMBER { nodename = pnapi::util::toString($1); }
                   ;

controlcommands:     /* emtpy */
                   | LCONTROL commands RCONTROL
                   ;

commands:            { yyerror("control commands not supported"); }
                   ;

markinglist:      /* empty */ 
                | marking
                | markinglist COMMA marking
                ;

marking:          nodeident COLON NUMBER { (PN->findPlace(nodename))->mark($3); }
                | nodeident              { (PN->findPlace(nodename))->mark(); }
                ;

finalmarkinglist: /* empty */
| finalmarking { //FIXME
		     }
                | finalmarkinglist COMMA finalmarking { 
		  //FIXME
		     }
                ;

finalmarking:     nodeident COLON NUMBER {
  /*(PN->findPlace(nodename))->isFinal = true;*/
		      finalMarking.insert(pair<Place *, unsigned int>
					  (PN->findPlace(nodename), $3)); }
                | nodeident {
		  /*(PN->findPlace(nodename))->isFinal = true;*/
		      finalMarking.insert(pair<Place *, unsigned int>
					  (PN->findPlace(nodename), 1)); }
                ;

port_area:        /* empty */
                | KEY_PORTS port_list
                ;

port_list:        port_definition
                | port_list port_definition
                ;

port_definition:  nodeident { current_port = *$1; } 
                  COLON port_participant_list SEMICOLON
                ;

port_participant_list: nodeident { 
  //FIXME: PN->findPlace(*$1)->setPort(current_port); 
		  }
                | port_participant_list COMMA nodeident { 
		      //FIXME: PN->findPlace(*$3)->setPort(current_port); 
		  }
                ;

transitionlist:   transitionlist transition
                | /* empty */
                ;

transition:       KEY_TRANSITION tname  { t = &PN->createTransition(nodename); }
                  annotation KEY_CONSUME        { readmode = 4; } 
                  arclist SEMICOLON KEY_PRODUCE { readmode = 5; } 
                  arclist SEMICOLON
                ;

tname:            IDENT  { nodename = *$1; }
                | NUMBER { nodename = pnapi::util::toString($1); }
                ;

annotation:       /* empty */
                | LCONTROL annotation_list RCONTROL
                ;

annotation_list:  IDENT { t->addLabel(*$1); }
                | IDENT { t->addLabel(*$1); } COMMA annotation_list
                ;

arclist:          /* empty */
                | arc 
                | arc COMMA arclist 
                ;

arc:              nodeident COLON NUMBER {
                         if (readmode == 4) 
			   PN->createArc(*PN->findPlace(nodename), *t, $3);
			 if (readmode == 5) 
			   PN->createArc(*t, *PN->findPlace(nodename), $3); }
                | nodeident {
		         if (readmode == 4) 
			   PN->createArc(*PN->findPlace(nodename), *t); 
			 if (readmode == 5) 
			   PN->createArc(*t, *PN->findPlace(nodename)); }
                ;

statepredicate: 
  /* empty */                          { $$ = NULL; }
| LPAR statepredicate RPAR             { $$ = $2; } 
| OP_NOT statepredicate                { $$ = new Node(Node::FORMULA_NOT, $2); }
| statepredicate OP_OR statepredicate  { $$ = new Node(Node::FORMULA_OR, $1, $3); }
| statepredicate OP_AND statepredicate { $$ = new Node(Node::FORMULA_AND, $1, $3); }
| statepredicate OP_AND KEY_ALL_OTHER_PLACES_EMPTY
                                       { $$ = new Node(Node::FORMULA_AAOPE, $1); }
| statepredicate OP_AND KEY_ALL_OTHER_INTERNAL_PLACES_EMPTY
                                       { $$ = new Node(Node::FORMULA_AAOIPE, $1); }
| statepredicate OP_AND KEY_ALL_OTHER_EXTERNAL_PLACES_EMPTY
                                       { $$ = new Node(Node::FORMULA_AAOEPE, $1); }
| nodeident OP_EQ NUMBER               { $$ = new Node(Node::FORMULA_EQ, $1, $3); }
| nodeident OP_NE NUMBER               { $$ = new Node(Node::FORMULA_NE, $1, $3); }
| nodeident OP_LT NUMBER               { $$ = new Node(Node::FORMULA_LT, $1, $3); }
| nodeident OP_GT NUMBER               { $$ = new Node(Node::FORMULA_GT, $1, $3); }
| nodeident OP_GE NUMBER               { $$ = new Node(Node::FORMULA_GE, $1, $3); }
| nodeident OP_LE NUMBER               { $$ = new Node(Node::FORMULA_LE, $1, $3); }
;


 /*****************************************************************************
  * C definitions
  ****************************************************************************/
%%

int yyerror(const char * msg)
{
  throw string(msg) + " (near '" + msg + "')";
}
