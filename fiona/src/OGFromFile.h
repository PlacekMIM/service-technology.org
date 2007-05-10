/*****************************************************************************
 * Copyright 2005, 2006, 2007 Jan Bretschneider, Peter Massuthe              *
 *                                                                           *
 * This file is part of Fiona.                                               *
 *                                                                           *
 * Fiona is free software; you can redistribute it and/or modify it          *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * Fiona is distributed in the hope that it will be useful, but WITHOUT      *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
 * more details.                                                             *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with Fiona; if not, write to the Free Software Foundation, Inc., 51       *
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.                      *
 *****************************************************************************/

/*!
 * \file    OGFromFile.h
 *
 * \brief   Holds an Operating Guidelines (OG) read from a OG file. We do not
 *          use the class OG for storing OGs read from a file because the class
 *          OG is tailored for computing an OG and therefore heavily depends
 *          on the existence of an underlying oWFN. If we used the class OG for
 *          storing an already computed OG, we would have to reimplement most
 *          methods anyway and the possibility of reuse would be minimal.
 *
 * \author  responsible: Jan Bretschneider <bretschn@informatik.hu-berlin.de>
 *
 * \note    This file is part of the tool Fiona and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universitšt zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 */

#ifndef OGFROMFILE_H_
#define OGFROMFILE_H_

#include <fstream>
#include <string>
#include <set>
#include <map>
#include <list>
#include "commGraphFormula.h"
#include "enums.h"

using namespace std;

class OGFromFileTransition;

class OGFromFileNode {
// order: public typedefs, private members and methods, public methods
public:
    typedef std::set<OGFromFileTransition*> transitions_t;
private:
    std::string name_;
    vertexColor color_;
    typedef std::map<std::string, OGFromFileNode*> transitionLabel2parentNode_t;
    transitionLabel2parentNode_t transitionLabel2parentNode;
    void addParentNodeForTransitionLabel(const std::string& transitionLabel,
        OGFromFileNode* parentNode);
    OGFromFileNode* getParentNodeForTransitionLabel(
        const std::string& transitionLabel) const;
    CommGraphFormula* annotation_;
    OGFromFileNode* depthFirstSearchParent;
public:
    transitions_t transitions;

    OGFromFileNode(const std::string& name_, CommGraphFormula* annotation_,
        vertexColor color = BLUE);

    ~OGFromFileNode();
    std::string getName() const;
    vertexColor getColor() const;
    bool isBlue() const;
    bool isRed() const;
    void addTransition(OGFromFileTransition* transition);
    bool hasTransitionWithLabel(const std::string& transitionLabel) const;
    bool hasBlueTransitionWithLabel(const std::string& transitionLabel) const;
    OGFromFileTransition* getTransitionWithLabel(
        const std::string& transitionLabel) const;
    OGFromFileNode* fireTransitionWithLabel(const std::string& transitionLabel);
    OGFromFileNode* backfireTransitionWithLabel(
        const std::string& transitionLabel) const;
    bool assignmentSatisfiesAnnotation(
        const CommGraphFormulaAssignment& assignment) const;

    std::string getAnnotationAsString() const;
	CommGraphFormula* getAnnotation() const;

	CommGraphFormulaAssignment* getAssignment() const;

    void removeTransitionsToNode(const OGFromFileNode* nodeToDelete);
    void setDepthFirstSearchParent(OGFromFileNode* depthFirstSearchParent);
    OGFromFileNode* getDepthFirstSearchParent() const;
};

class OGFromFileTransition {
private:
    OGFromFileNode* src;
    OGFromFileNode* dst;
    std::string label;
public:
    OGFromFileTransition(OGFromFileNode* src, OGFromFileNode* dst,
        const std::string& label);
    const std::string getLabel();
    bool hasLabel(const std::string& label_) const;
    OGFromFileNode* getDst() const;
    OGFromFileNode* getSrc() const;
};

class oWFN;

class OGFromFile {
private:
    OGFromFileNode* root;
    typedef std::set<OGFromFileNode*> nodes_t;
    nodes_t nodes; // needed for proper deletion of OG.
    typedef nodes_t::const_iterator nodes_const_iterator;
    typedef nodes_t::iterator nodes_iterator;

    void removeTransitionsToNodeFromAllOtherNodes(
        const OGFromFileNode* nodeToDelete);

    /**
     * Recursive coordinated dfs through OG and rhs OG.
     * \param currentOGNode the current node of the OG
     * \param currentRhsNode the current node of the rhs OG
     * \param productOG the resulting product OG
     */
    void buildProductOG(OGFromFileNode* currentOGNode,
        OGFromFileNode* currentRhsNode, OGFromFile* productOG);

	bool simulatesRecursive ( OGFromFileNode *myNode, 
							  set<OGFromFileNode*> *myVisitedNodes, 
							  OGFromFileNode *simNode,
							  set<OGFromFileNode*> *simVisitedNodes);
public:
    OGFromFile();
    ~OGFromFile();
    void addNode(OGFromFileNode* node);
    OGFromFileNode* addNode(const std::string& nodeName,
        CommGraphFormula* annotation, vertexColor color = BLUE);
    void addTransition(const std::string& srcName, const std::string& dstName,
        const std::string& label);
    bool hasNodeWithName(const std::string& nodeName) const;
    OGFromFileNode* getRoot() const;
    void setRoot(OGFromFileNode* newRoot);
    void setRootToNodeWithName(const std::string& nodeName);
    OGFromFileNode* getNodeWithName(const std::string& nodeName) const;
    bool hasNoRoot() const;

    /**
     * @todo Remove unreachable nodes after (or during) removal of false
     * nodes.
     */
    void removeFalseNodes();

    /**
     * Type of container passed to OGFromFile::product().
     */
    typedef std::list<OGFromFile*> ogs_t;

    /**
     * Type of container passed to OGFromFile::getProductOGFilePrefix().
     */
    typedef std::list<std::string> ogfiles_t;

    /**
     * Returns the product OG of all given OGs. The caller has to delete the
     * returned OGFromFile.
     * \param ogs Given OGs. Must contain at least two OG.
     */
    static OGFromFile* product(const ogs_t& ogs);

    /**
     * Returns the product OG of this OG and the passed one. The caller has to
     * delete the returned OGFromFile.
     */
    OGFromFile* product(const OGFromFile* rhs);

    /**
     * Produces from the given OG file names the default prefix of the
     * product OG output file.
     */
    static std::string getProductOGFilePrefix(const ogfiles_t& ogfiles);

    /**
     * Enforces the current OG to respect the given constraint.
     * \return the OG respecting the constraint
     * \param constraint the constraint to be enforced
     */
    OGFromFile* enforce(const OGFromFile*);

    /**
     * Strips the OG file suffix from filename and returns the result.
     */
    static std::string stripOGFileSuffix(const std::string& filename);

    void printGraphToDot(OGFromFileNode* v, fstream& os, std::map<OGFromFileNode*, bool>&) const;

    void printDotFile() const;
    void printDotFile(const std::string& filenamePrefix) const;
    void printDotFile(const std::string& filenamePrefix,
        const std::string& dotGraphTitle) const;

    bool simulates ( OGFromFile *simulant );
};


#endif
