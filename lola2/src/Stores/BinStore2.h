/*!
\file BinStore.h
\author Karsten, Christian Koch
\status new
\brief implement Store as a decision tree with bitvectors.
*/

#pragma once
#include "Core/Dimensions.h"
#include "Stores/Store.h"

typedef uint32_t vectordata_t;



// maximum size (in bits) of a place
const int PLACE_WIDTH = sizeof(capacity_t) * 8;

// maximum size (in bits) of a vector item
const int VECTOR_WIDTH = sizeof(vectordata_t) * 8;


class BinStore2 : public Store
{
public:
    BinStore2();
    ~BinStore2();

    /// check whether current marking is stored
    virtual bool searchAndInsert();

    /// check whether current marking is sted and return state
    virtual bool searchAndInsert(State** s);
    //void pbs(unsigned int, unsigned int, vectordata_t*, void*);
    //void printBinStore();
private:
    /// a binary decision node
    class Decision
    {
    public:
        Decision(bitindex_t);
        bitindex_t bit;
        vectordata_t* vector;
        Decision* nextold;
        Decision* nextnew;
        State* state;
        ~Decision();

    };
    // first branch in decision tree; NULL as long as less than two elements in bucket
    Decision** branch;

    // first vector in bucket; null as long as bucket empty
    vectordata_t** firstvector;

    // precalculated bitmask for bit filtering: place_bitmask[i] gives the i least significant bits
    capacity_t place_bitmask[PLACE_WIDTH+1];

    // precalculated bitmask for bit filtering: vector_bitmask[i] gives the i least significant bits
    vectordata_t vector_bitmask[VECTOR_WIDTH+1];
};
