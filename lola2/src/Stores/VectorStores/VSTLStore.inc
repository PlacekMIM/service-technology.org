/*!
\author Niels, Christian Koch
\file VSTLStore.cc
\status new
*/

#include <algorithm>
#include <Net/Net.h>
#include <Net/Place.h>
#include <Net/Marking.h>
#include <Stores/VectorStores/VSTLStore.h>

template <typename T>
VSTLStore<T>::VSTLStore(index_t num_threads)
{
    singleThreaded = (num_threads == 1);
    intermediate = new std::vector<vectordata_t>[num_threads];
    pthread_rwlock_init(&rwlock, NULL);
}

template <typename T>
VSTLStore<T>::~VSTLStore()
{
    pthread_rwlock_destroy(&rwlock);
    delete[] intermediate;
}

template <typename T>
bool VSTLStore<T>::searchAndInsert(const vectordata_t* in, bitindex_t bitlen, hash_t hash, T** payload, index_t threadIndex)
{
    index_t vectorLength = (bitlen+(VECTOR_WIDTH-1))/VECTOR_WIDTH;

    std::vector<vectordata_t> &ci = intermediate[threadIndex];

    ci.resize(vectorLength);
    // copy current marking to vector
    std::copy(in, in + vectorLength, ci.begin());

    // shift away unused bits to make sure they are zero
    bitlen -= vectorLength * VECTOR_WIDTH;
    if(bitlen)
        (ci[vectorLength-1] >>= (VECTOR_WIDTH-bitlen)) <<= (VECTOR_WIDTH-bitlen);

    int ret;
    // avoid locking overhead if working in single-threaded mode
    if(singleThreaded) {
    	ret = store.count(ci);
    	*payload = &store[ci];
        return ret;
    }

    // test if already inserted
    pthread_rwlock_rdlock(&rwlock);
    ret = store.count(ci);
    pthread_rwlock_unlock(&rwlock);
    if(ret) {
        return true;
    }

    // add vector to marking store
    pthread_rwlock_wrlock(&rwlock);
	ret = store.count(ci);
	*payload = &store[ci];
    pthread_rwlock_unlock(&rwlock);

    return ret;
}

/*!
 * \brief  gets and removes a vector from the store
 * \note   Assumption: Removing of states is done by a single thread and after inserting states
 *         hence no locking will be done and the internal cache of the first thread will be used.
 */
template <typename T>
bool VSTLStore<T>::popVector(vectordata_t * & out)
{
  // get result
  bool result = !store.empty();

  // when store not empty, set out pointer to vector of this state
  if(result)
  {
    __typeof(store.begin()) it = store.begin(); // get iterator of first element
    std::vector<vectordata_t> &ci = intermediate[0] = it->first; // get its vector
    store.erase(it); // remove this iterator from the store
    out = &ci[0]; // return underlying array
  }

  // return result
  return result;
}