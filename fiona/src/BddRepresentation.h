/*!
 *  \class BddRepresentation
    \author Kathrin Kaschner
 */
#ifndef BDDREPRESENTATION_H_
#define BDDREPRESENTATION_H_
 
#include "mynew.h" 
#include <vector>
#include <map>

#include "vertex.h"
#include "CNF.h"

#include "cudd.h" 
#include "cuddInt.h"

using namespace std;
typedef vector<bool> BitVector;

class BddLabelTab;

class BddRepresentation{
	public:
		BddRepresentation(vertex * v, unsigned int numberOfLabels, Cudd_ReorderingType heuristic = CUDD_REORDER_SIFT);
		~BddRepresentation();
		
		void addOrDeleteLeavingEdges(vertex* v);
		void generateRepresentation(vertex* v, bool visitedNodes[]);
		void reorder(Cudd_ReorderingType heuristic = CUDD_REORDER_SIFT);
		void print();
		void printDotFile(char** names= NULL);

		
        // Provides user defined operator new. Needed to trace all new
        // operations on this class.
#undef new
        NEW_OPERATOR(BddRepresentation)
#define new NEW_NEW

	private:	 
		DdManager* mgrMp;
		DdManager* mgrAnn;
		DdNode* bddMp;
		DdNode* bddAnn;
		int maxNodeBits;
		int maxLabelBits;
		unsigned int maxNodeNumber;
		unsigned int nbrLabels;
		map<unsigned int, unsigned int> nodeMap;
		BddLabelTab * labelTable;
		
		DdNode* nodesToBddMp(unsigned int node1, unsigned int node2);
		DdNode* labelToBddMp(char* label);
		DdNode* annotationToBddAnn(vertex * v);
		DdNode* CNFtoBddAnn(CNF * cl);
		BitVector numberToBin(unsigned int number, int cntBits);
		unsigned int getBddNumber(unsigned int node);
		void addBddVars(unsigned int max);
		//DdNode* annotationToBddAnn(DdManager* mgr, int nodeNumber, DdNode* annotation, int maxNodeBits);
		//DdNode* labelToBddAnn(DdManager* mgr, char* channel, int first, int count, vector<char*> v_channel);
		
		int nbrBits(unsigned int i);
		string myitoa(unsigned int value, int base);
		
		void checkManager(DdManager* mgr, char* table);
};

#endif /*BDDREPRESENTATION_H_*/
