#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "hash.h"
#include <iostream>
#include <time.h>

class BloomFilter
{
    private:
    /**
     * k = the number of hashfunctions
     * MAX_CAPACITY = the number of bits in the Bloom filter
     * LOG_2_RANGE = the log (base 2) of MAX_CAPACITY
     * hash_func_rands = the random byte to be appended to the key before hashing
     * filter = the bloom filter (bit-array)
     */
    unsigned k;
    const unsigned MAX_CAPACITY = 1024;
    const unsigned LOG_2_RANGE = 10;
    char* hash_func_rands;
    bool* filter;

    public:

    /**
     * @brief Constructor to initialize the filter
     */
    BloomFilter(unsigned k);

    /**
     * @brief Destructor to delete the bit array and hash-function random bytes
     */
    ~BloomFilter();

    /**
     * @brief Inserts the key into the Bloom filter
     * 
     * @param key : the key of the element to be added
     * @return ** void 
     */
    void insert(char* key);

    /**
     * @brief Checks if the key is present in the filter
     * 
     * @param key : the key of the element whose presence is to be checked
     * @return true : if the key exists
     * @return false : if the key doesn't exist
     */
    bool containsKey(char* key);

};

BloomFilter::BloomFilter(unsigned k)
{
    this->k = k;
    srand(time(NULL));

    filter = new bool[MAX_CAPACITY];
    std::fill(filter, filter + MAX_CAPACITY, false);

    hash_func_rands = new char[k];
    for(unsigned i=0;i<k;i++)
        hash_func_rands[i] = rand() % 47;
}

BloomFilter::~BloomFilter()
{
    delete[] filter;
    delete[] hash_func_rands;
}

void BloomFilter::insert(char* key)
{
    for(unsigned i=0;i<k;i++)
    {
        unsigned hash = get_hash(key, hash_func_rands[i], LOG_2_RANGE);
        filter[hash] = true;
    }
}

bool BloomFilter::containsKey(char* key)
{
    for(unsigned i=0;i<k;i++)
    {
        unsigned hash = get_hash(key, hash_func_rands[i], LOG_2_RANGE);
        if(!filter[hash])
            return false;
    }
    return true;
}

#endif