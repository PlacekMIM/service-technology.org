/*****************************************************************************\
 Safira -- Implementing a Set Algebra for Service Behavior

 Copyright (c) 2010 Kathrin Kaschner

 Safira is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 Safira is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with Safira.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef GRAPHINTERSECTION_H_
#define GRAPHINTERSECTION_H_

#include "Graph.h"
#include "stack"

using namespace std;

class GraphIntersection : public Graph {
	private:
		list<Intpair> initialNodepairs;
		stack<long> myStack;
		stack<Nodepair*> unproc;

//        void printNodepairs(ostream& o) const;

		/// prints the global Formula of the complement (it is required that makeTotal and makeComplete was executed before)
//        void printGlobalFormula(ostream& o) const;
        void product(Nodepair* qp);

        ///converts the map of Nodepairs to a map of Nodes
        void convertTheMap();

    public:

        /// the nodes indexed by an identifier
        map<Intpair, Nodepair*, Cmp> nodepairs;

    	///constructor
        GraphIntersection();

    	///destructor
        ~GraphIntersection();

    	/// cumputes the intersection for the given graphs
    	void intersection(Graph *g1, Graph *g2);

    	void generateGlobalFormula(const Graph *g1, const Graph *g2);

    	///converts "this" to a Graph object g
    	void convertToGraph(Graph *g);

//        /// Graphviz dot output
//        void toDot(FILE* out, string title = "") const;
//
//        /// Graph output
//        void print(ostream& o) const;
};

#endif /* GRAPHINTERSECTION_H_ */
