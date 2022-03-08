#ifndef HASH_H
#define HASH_H

#include <string.h>
#include <stdint.h>
#include <memory.h>

#define FORCE_INLINE inline __attribute__((always_inline))

typedef uint8_t BYTE;
typedef uint32_t WORD;

FORCE_INLINE WORD murmur_32_scramble(WORD key_chunk)
{
    key_chunk *= 0xcc9e2d51;
    key_chunk = (key_chunk << 15) | (key_chunk >> 17);
    key_chunk *= 0x1b873593;
    return key_chunk;
}

/**
 * MurmurHash3 was written by Austin Appleby in 2008 and is currently hosted on GitHub along with its test suite named 'SMHasher'
 */
WORD murmur3_32(const BYTE* key, size_t len, WORD seed)
{
    WORD h = seed;
    WORD k;

    for(size_t i = len >> 2; i; i--)
    {
        memcpy(&k, key, sizeof(WORD));
        key += sizeof(WORD);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    k = 0;
    for(size_t i = len & 3; i; i--)
    {
        k <<= 8;
        k |= key[i - 1];
    }

    h ^= murmur_32_scramble(k);
    h ^= len;
    h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

/**
 * @brief Get the hash value of the string str
 * 
 * @param str : the key to be hashed
 * @param hash_rand : a random value to be appended to the end of the string before hashing
 * @param log2range : the log (base 2) of the maximum capacity of the hashset or hastable
 * @return ** unsigned : a hash value between 0 and 2^log2range
 */
unsigned get_hash(const char* str, char hash_rand, unsigned log2range)
{
    WORD seed = 0x9747b28c;
    size_t len = strlen(str) + 1;
    char str_to_hash[len];

    strcpy(str_to_hash, str);

    str_to_hash[len - 1] = hash_rand;

    WORD hash = murmur3_32((BYTE*)str_to_hash, len*sizeof(char), seed);   

    hash = hash << (32 - log2range);
    hash = hash >> (32 - log2range);
    return hash;
}

#endif
