#include <config.h>
#include "testGraph.h"
#include "Graph.h"
#include <string>
#include <iostream>
#include "Formula.h"
#include "Node.h"
#include "cmdline.h"
#include "testFormula.h"

//#include "cmdline.h"
//#include "verbose.h"
//#include "config-log.h"

#include "Output.h"

/// the command line parameters
gengetopt_args_info args_info;

using std::map;
using std::string;
using std::vector;

extern map<string, unsigned int> label2id;
extern map<unsigned int, string> id2label;

extern unsigned int firstLabelId; //all labels including tau
extern unsigned int firstInputId; //input labels
extern unsigned int firstOutputId;//output labels

extern unsigned int lastLabelId;
extern unsigned int lastInputId;
extern unsigned int lastOutputId;

void initGraph(){
	initFormulaClass();
	Node::init();

	firstLabelId = 2;
	firstInputId = 3;
	lastInputId = 4;
	firstOutputId = 5;
	lastOutputId = 6;
	lastLabelId = 6;

	cout << "initGraphClass... \t passed" << endl;

}


void testGraphClass(){
	testComplement();
	cout << "testGraphClass... \t passed" << endl;

}

/* computes the complement of a given annotated Graph
 * the correctness of the result is checked by the testsuite.at
 * the Funtion make complete is also tested by this method
 */
void testComplement(){
	initGraph();

	Formula* f0 = new FormulaOR(new FormulaLit(label2id["I"]), new FormulaLit(label2id["O"])); //f1 = I+O
	Formula* f1 = new FormulaOR(new FormulaLit(label2id["O"]), new FormulaFinal()); //f2 = O + final


	Node* n0 = new Node(f0, 0);
	Node* n1 = new Node(f1, 1);

	assert(n0->id == 0);
	assert(n1->id == 1);

	assert(n0->formula->toString() == "(I + O)");
	assert(n1->formula->toString() == "(O + final)");

	n0->addEdge(label2id["I"], n0); //n0--I->n0
	n0->addEdge(label2id["I"], n1); //n0--I->n1
	n0->addEdge(label2id["O"], n1); //n0--O->n1
	n1->addEdge(label2id["O"], n0); //n1--O->n1

	Graph* g = new Graph();
	g->nodes[n0->id] = n0;
	g->nodes[n1->id]  = n1;
	g->addInitialNode(n0->id);
	g->globalFormula = new FormulaOR(new FormulaNUM(n0->id), new FormulaNUM(n1->id)); // 0+1

	g->makeTotal();
	Output o1("testGraph_total.eaa", "total testGraph");
	g->print(o1);
	g->makeComplete();
	Output o2("testGraph_complement.eaa", "complement testGraph");
	g->printComplement(o2);

	assert(g->getSizeOfAddedNodes() == 5);
	//g->printNodes();

	delete g;
}