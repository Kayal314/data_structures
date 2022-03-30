#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class Trie
{
    private:
    struct Node;

    unsigned size;
    Node* root;

    static const unsigned ALPHABET_SIZE = 26;

    public:
    /**
     * @brief Constructor to initialize an empty symbol table
     * 
     */
    Trie();

    /**
     * @brief Destructor to delete dynamically allocated memory
     * 
     */
    ~Trie();

    /**
     * @brief Returns the number of keys in the symbol table
     * 
     * @return ** unsigned : the size of the symbol table
     */
    unsigned get_size();

    /**
     * @brief Checks whether the symbol table contains the given key
     * 
     * @param key : the key to be searched in the table
     * @return true : if the symbol table contains the key
     * @return false : if the symbol table does not contain the key
     */
    bool contains(string key);

    /**
     * @brief Returns the value associated with the given key
     * 
     * @param key : the key whose associated value is to be found
     * @return ** T : the value associated with the given key
     */
    T get(string key);

    /**
     * @brief Inserts a new key into the symbol table along with its
     * associated value. It overwrites the old value with the new value 
     * if the key is already present in the table
     * 
     * @param key : the key to be inserted
     * @param val : the value associated with the key
     * @return ** void 
     */
    void insert(string key, T val);

    /**
     * @brief Returns the string in the symbol table that is the longest
     * prefix of the the given query string
     * 
     * @param query : the query string
     * @return ** string : the string in the symbol tbale that is the longest
     * prefix of the query string
     */
    string longest_prefix(string query);

    /**
     * @brief Removes the key from the symbol table, if present
     * 
     * @param key : the key to be removed
     * @return ** T : the value associated with the given key
     */
    T remove(string key);

    private:
    /**
     * @brief Helper function to delete nodes in a post-order fashion
     * 
     * @param node : the tree to be deleted in post-order function (recursive)
     * @return ** void 
     */
    void release_mem(Node* node);

    /**
     * @brief Helper function (recursive) to remove key from the symbol table
     * 
     * @param node : the table (recursive) from which the characters have to be removed
     * @param key : the key to be removed (if present)
     * @param key_ind : the character index of the key under consideration
     * @return ** T 
     */
    T remove(Node* node, string key, unsigned key_ind);

    /**
     * @brief Helper function to check if a node is childless
     * 
     * @param node : the node to be checked
     * @return true : if the node has no child
     * @return false : if the node has atleast one child
     */
    bool has_no_children(Node* node);
 
};

template <typename T>
class Trie<T>::Node
{
    private:
    char ch;
    Node** children;
    T val;
    bool is_end;
    Node();
    Node(char ch);
    friend class Trie<T>;
};

template <typename T>
Trie<T>::Node::Node()
{
    this->children = new Node*[ALPHABET_SIZE];
    for(unsigned i = 0; i < ALPHABET_SIZE; i++)
        children[i] = nullptr;
    this->ch = 0;
    this->is_end = false;
    this->val = T{};
}

template <typename T>
Trie<T>::Node::Node(char ch)
{
    this->children = new Node*[ALPHABET_SIZE];
    for(unsigned i = 0; i < ALPHABET_SIZE; i++)
        children[i] = nullptr;
    this->ch = ch;
    this->is_end = false;
    this->val = T{};
}

template <typename T>
Trie<T>::Trie()
{
    root = new Node();
    size = 0;
}

template <typename T>
Trie<T>::~Trie()
{
    if(root)
        release_mem(root);
}

template <typename T>
void Trie<T>::release_mem(Node* node)
{
    for(unsigned i = 0; i < ALPHABET_SIZE; i++)
        if(node->children[i])
            release_mem(node->children[i]);
    delete[] node->children;
    delete node;
}

template <typename T>
unsigned Trie<T>::get_size()
{
    return size;
}

template <typename T>
void Trie<T>::insert(string key, T val)
{
    if(key.length() == 0)
        throw invalid_argument("Key must not be empty");
    Node* node = root;
    bool not_present = false;
    for(unsigned i = 0; i < key.length(); i++)
    {
        char ch = key.at(i);
        unsigned index = ch - 'a';
        if(!node->children[index])
        {
            node->children[index] = new Node(ch);
            not_present = true;
        }
        
        node = node->children[index];
    }
    not_present = not_present || !node->is_end;
    if(not_present)
        size++;
 
    node->val = val;
    node->is_end = true;
}

template <typename T>
bool Trie<T>::contains(string key)
{
    Node* node = root;
    for(unsigned i = 0; i < key.length(); i++)
    {
        char ch = key.at(i);
        unsigned index = ch - 'a';
        if(!node->children[index])
            return false;
        node = node->children[index];
    }
    return node->is_end;
}

template <typename T>
T Trie<T>::get(string key)
{
    Node* node = root;
    for(unsigned i = 0; i < key.length(); i++)
    {
        char ch = key.at(i);
        unsigned index = ch - 'a';
        if(!node->children[index])
        {
            cout<<"The queried key is not present in the symbol table"<<endl;
            return T{};
        }
        node = node->children[index];
    }
    if(node->is_end)
        return node->val;
    cout<<"The queried key is not present in the symbol table"<<endl;
    return T{};
}

template <typename T>
string Trie<T>::longest_prefix(string query)
{
    int len = -1;
    Node* node = root;
    for(unsigned i = 0; i < query.length(); i++)
    {
        char ch = query.at(i);
        unsigned index = ch - 'a';

        if(!node->children[index])
            break;
        else if(node->children[index]->is_end)
            len = i;
        node = node->children[index];
    }
    return query.substr(0, len + 1);
}

template <typename T>
T Trie<T>::remove(string key)
{
    if(key.length() == 0)
        throw invalid_argument("Key must not be empty");
    size--;
    return remove(root, key, 0);
}

template <typename T>
T Trie<T>::remove(Node* node, string key, unsigned key_ind)
{
    if(!node)
    {
        // since the key doesn't exist, undo the size decrement
        size++;
        return T{};
    }

    if(key_ind == key.length())
    {
        node->is_end = false;
        T to_return = node->val;
        node->val = T{};
        if(has_no_children(node))
            delete node;
        return to_return;
    }

    unsigned i = key.at(key_ind) - 'a';
    T val = remove(node->children[i], key, key_ind + 1);

    if(has_no_children(node) && !node->is_end)
        delete node;
    return val;
}

template <typename T>
bool Trie<T>::has_no_children(Node* node)
{
    for(unsigned i = 0; i < ALPHABET_SIZE; i++)
        if(node->children[i])
            return false;
    return true;
}

#endif



