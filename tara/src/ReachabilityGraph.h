#ifndef REACHABILITYGRAPH_H
#define REACHABILITYGRAPH_H

#include <map>
#include <vector>
#include <set>
#include <string>
#include "Multiset.h"
#include "Situation.h"

class MarkingCondition;

class ReachabilityGraph {
  public:
  int root;
  std::map<int, std::map<int, int> > delta;
  std::map<int, std::map<int,int> > tokens;
  std::vector<char*> places;
  
  std::set<int> states;
  void removeNode(int s);
  
  int insertPlace(char* p);
  int getTokens(int state, int place);
    
  std::set<int> enabledTransitions(int s) const;
  bool enables(int s, int t) const;
  int yields(int s, int t) const;  
  
  virtual void print();
  virtual void print_r(int s, std::set<int>& visited);

  std::string markingString(int s);

  std::set<int> reachableNodes(int state);

  //void removeIndifferents(MarkingCondition& condition);
};

#endif
