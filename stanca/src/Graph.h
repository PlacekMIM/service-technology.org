#ifndef _GRAPH_H
#define _GRAPH_H

#include <map>
#include "Node.h"
#include "types.h"
#include <iostream>

using namespace std;

class Graph {
	private:
		list<int> initialNodes;
		map<int, Node*> addedNodes;
		Node *trap;

        void printInterface(ostream& o) const;
        void printInitialNodes(ostream& o) const;
        void printNodes(ostream& o) const;
        /// prints the global Formula of the complement (it is required that makeTotal and makeComplete was executed before)
        void printGlobalFormulaForComplement(ostream& o) const;
        void printGlobalFormula(ostream& o) const;

    public:
        /// the nodes indexed by an identifier
        map<int, Node*> nodes;

        /// the global formula of the graph
        Formula *globalFormula;

    	///constructor
    	Graph();

    	///destructor
    	~Graph();

    	/// add an initial node to the grpah
    	void addInitialNode(int id);

    	/// computes the complement for the extended annotated graph
    	void complement();

        /// complete the extended annotated graph
        void makeComplete();

        /// make the extended annotated graph total
        void makeTotal();

        /// Graphviz dot output
        void toDot(FILE* out, string title = "") const;
       // void toDot(string filename, string title = "") const;

        /// Graph output
        void print(ostream& o) const;

        /// Graph output as complement (it is required that makeTotal and makeComplete was executed before)
        void printComplement(ostream& o) const;

        string getGlobalFormulaForComplement() const;
        string getGlobalFormula() const;

        /// get the number of new nodes in the complement
        int getSizeOfAddedNodes();


//        /// adds a label to the graphs
//        void addLabel(string label, unsigned int id);

};

#endif