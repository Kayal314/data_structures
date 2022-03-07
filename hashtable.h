#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string.h>
#include "hash.h"
#include "time.h"

/**
 * @brief Each item in the hastable contains and string key and data of type T
 *        Each item contains a pointer to the next item who's key has the same hash value
 */
template <typename T>
struct Item
{
    T data;
    char* key;
    Item<T>* next;
    Item()
    {
        this->data=T{};
        this->key=nullptr;
        this->next=nullptr;
    }
    Item(T data, char* key)
    {
        this->data = data;
        this->key = key;
        this->next = nullptr;
    }
    ~Item()
    {
        delete[] key;
    }
};

/**
 * Collision is handled using chaining
 */
template <typename T>
class Hashtable
{
    private:
    Item<T>** table;
    const unsigned MAX_SIZE = 1024;
    const unsigned LOG_2_RANGE = 10;
    char rand_val;
    unsigned size;
    unsigned collisions;

    public:
    /**Constructor**/
    Hashtable();

    /**Destructor**/
    ~Hashtable();

    /**
     * @brief Inserts an item (data, key) into the hashtable in O(1) time (worst-case)
     * 
     * @param data The data which the item holds
     * @param key The key of the item
     * @return ** void 
     */
    void insert(T data, char* key);

    /**
     * @brief Removes the item (if present) with the given key in O(1) amortized-time
     * 
     * @param key The key of the item to be removed
     * @return ** T : The data stored in the item to be removed
     */
    T remove(char* key);

    /**
     * @brief Checks if any item with the given key is present in the hashtable in O(1) amortized-time
     * 
     * @param key The key of the item whose presense is to be checked
     * @return true : If the item is present
     * @return false : If the item is not present
     */
    bool containsKey(char* key);

    /**
     * @brief Get the number of elements currently stored in the hashtable;
     * 
     * @return ** unsigned : The number of elements in the hashtable
     */
    unsigned get_size();

    /**
     * @brief Replaces the data stored by an item (with the given key) by some new data
     * 
     * @param rep : The new data to be stored in a particular item
     * @param key : The key of the item whose data is to be altered
     * @return true : If the item exists
     * @return false : If the item doesn't exist
     */
    bool replace(T rep, char* key);

    /**
     * @brief Get the number of collisions that have occured till now
     * 
     * @return ** unsigned : The number of collisions
     */
    unsigned get_collisions();
    
};


template <typename T>
Hashtable<T>::Hashtable()
{
    srand(time(NULL));
    rand_val = rand() % 47;
    table = new Item<T>*[MAX_SIZE];
    for(unsigned i=0;i<MAX_SIZE;i++)
        table[i]=nullptr;
    size = 0;
    collisions = 0;
}


template <typename T>
Hashtable<T>::~Hashtable()
{
    for(unsigned i=0;i<MAX_SIZE;i++)
    {
        Item<T>* item = table[i];
        while(item)
        {
            Item<T>* to_del = item;
            item = item->next;
            delete to_del;
        }
    }
}

template <typename T>
void Hashtable<T>::insert(T data, char* key) // O(1) worst-case
{
    unsigned hash = get_hash(key, rand_val, LOG_2_RANGE);
    Item<T>* bucket = table[hash];
    if(bucket)
        collisions++;
    Item<T>* item = new Item<T>(data, key);
    item->next = bucket;
    table[hash] = item;
    size++;
}

template <typename T>
T Hashtable<T>::remove(char* key) // O(1) average
{
    unsigned hash = get_hash(key, rand_val, LOG_2_RANGE);
    Item<T>* prev = table[hash];
    if(!prev)
    {
        std::cout<<"The key does not exist in the hashtable"<<std::endl;
        return T{};
    }
    Item<T>* bucket = table[hash]->next;
    if(strcmp(prev->key, key) == 0)
    {
        T to_del = prev->data;
        table[hash] = bucket;
        delete prev;
        size--;
        return to_del;
    }
    while(bucket)
    {
        if(strcmp(bucket->key, key) == 0)
        {
            prev->next=bucket->next;
            T to_del = bucket->data;
            delete bucket;
            size--;
            return to_del;
        }
        bucket = bucket->next;
        prev = prev->next;
    }
    std::cout<<"The key does not exist in the hashtable"<<std::endl;
    return T{};
}

template <typename T>
bool Hashtable<T>::containsKey(char* key) // O(1) average
{
    unsigned hash = get_hash(key, rand_val, LOG_2_RANGE);
    Item<T>* bucket = table[hash];
    while(bucket)
    {
        if(strcmp(bucket->key, key) == 0)
            return true;
        bucket = bucket->next;
    }
    return false;
}

template <typename T>
unsigned Hashtable<T>::get_size()
{
    return this->size;
}

template <typename T>
bool Hashtable<T>::replace(T rep, char* key)
{
    unsigned hash = get_hash(key, rand_val, LOG_2_RANGE);
    Item<T>* bucket = table[hash];
    while(bucket)
    {
        if(strcmp(bucket->key, key) == 0)
        {
            bucket->data = rep;
            return true;
        }
        bucket = bucket->next;
    }
    return false;
}

template <typename T>
unsigned Hashtable<T>::get_collisions()
{
    return this->collisions;
}

#endif