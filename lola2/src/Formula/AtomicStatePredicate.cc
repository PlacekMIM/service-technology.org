/*!
\file AtomicStatePredicate.cc
\author Karsten
\status new

\brief class implementation for atomic state predicates
*/

#include "Formula/AtomicStatePredicate.h"
#include "Net/Net.h"
#include "Net/Marking.h"

AtomicStatePredicate::AtomicStatePredicate(index_t p, index_t n, int k)
{
	posPlaces = (index_t *) malloc(p * SIZEOF_INDEX_T);
	negPlaces = (index_t *) malloc(n * SIZEOF_INDEX_T);
	posMult = (capacity_t *) malloc(p * SIZEOF_CAPACITY_T);
	negMult = (capacity_t *) malloc(n * SIZEOF_CAPACITY_T);
	cardPos = p;
	cardNeg = n;
	threshold = k;
	
	// collect up set

	index_t * up1 = (index_t *) malloc(Net::Card[TR] * SIZEOF_INDEX_T);
	index_t * up2 = (index_t *) malloc(Net::Card[TR] * SIZEOF_INDEX_T);
	cardUp = 0;
	
	// idea: for each p: merge up1 and post places of pos / pre places of neg into up2
	for(index_t i = 0; i < cardPos; i++)
	{
		const index_t p = posPlaces[i];
		const index_t cardP = Net::CardArcs[PL][POST][p];

		index_t a = 0; // index in up1
		index_t b = 0; // index in arc list of place
		index_t c = 0; // index in arc list of place
	
		while(a < cardUp && b < cardP)
		{
			if(up1[a] < Net::Arc[PL][POST][p][b])
			{
				up2[c++] = up1[a++];
				continue;
			}
			if(up1[a] > Net::Arc[PL][POST][p][b])
			{
				up2[c++] = Net::Arc[PL][POST][p][b++];
				continue;
			}
			assert(up1[a] == Net::Arc[PL][POST][p][b]);
			up2[c++] = up1[a++];
			++b;
		}
		for(;a < cardUp; a++)
		{
			up2[c++] = up1[a++];
		}
		for(;b < cardUp; a++)
		{
			up2[c++] = Net::Arc[PL][POST][p][b];
		}
		index_t * tmp = up1;
		up1 = up2;
		up2 = up1;
		cardUp = c;
	}
	for(index_t i = 0; i < cardNeg; i++)
	{
		const index_t p = negPlaces[i];
		const index_t cardP = Net::CardArcs[PL][PRE][p];

		index_t a = 0; // index in up1
		index_t b = 0; // index in arc list of place
		index_t c = 0; // index in arc list of place
	
		while(a < cardUp && b < cardP)
		{
			if(up1[a] < Net::Arc[PL][PRE][p][b])
			{
				up2[c++] = up1[a++];
				continue;
			}
			if(up1[a] > Net::Arc[PL][PRE][p][b])
			{
				up2[c++] = Net::Arc[PL][PRE][p][b++];
				continue;
			}
			assert(up1[a] == Net::Arc[PL][PRE][p][b]);
			up2[c++] = up1[a++];
			++b;
		}
		for(;a < cardUp; a++)
		{
			up2[c++] = up1[a++];
		}
		for(;b < cardUp; a++)
		{
			up2[c++] = Net::Arc[PL][PRE][p][b];
		}
		index_t * tmp = up1;
		up1 = up2;
		up2 = up1;
		cardUp = c;
	}
	up = (index_t *) realloc(up1,cardUp * SIZEOF_INDEX_T); 
	free(up2);
}

void AtomicStatePredicate::addPos(index_t i, index_t p, capacity_t m)
{
	posPlaces[i] = p;
	posMult[i] = m;
}

void AtomicStatePredicate::addNeg(index_t i, index_t p, capacity_t m)
{
	negPlaces[i] = p;
	negMult[i] = m;
}

index_t AtomicStatePredicate::getUpSet(index_t * stack, bool * onstack)
{
	index_t stackpointer = 0;
	for(index_t i = 0; i < cardUp; i++)
	{
		index_t element;
		if(!onstack[element= up[i]])
		{	
			stack[stackpointer++] = element;
		}
	}
	return stackpointer;
} 
                                 
	
void AtomicStatePredicate::update(int delta)
{
	sum += delta;
	if(sum <= threshold && !value)
	{
		value = true;
		parent -> updateFT(position); 
		return;
	}
	if(sum > threshold && value)
	{
		value = false;
		parent -> updateTF(position); 
		return;
	}
}

void AtomicStatePredicate::evaluate()
{
	sum = 0;
	for(index_t i = 0; i < cardPos; ++i)
	{
		sum += Marking::Current[posPlaces[i]] * posMult[i];
	}
	for(index_t i = 0; i < cardNeg; ++i)
	{
		sum -= Marking::Current[negPlaces[i]] * negMult[i];
	}
	if(sum <= threshold)
	{
		value = true;
	}
	else
	{
		value = false;
	}
}
