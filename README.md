Sparse_Array

This project aims to implement the sparse array in C.

Sparse array is defined as a data structure that is sparsely filled. We want to be able to access the array via indices. And the complexity of indexing should be constant.

Design:
Trie is used. Assume the index is by a unsigned int array (32-bit). Therein, each 8 bits are used for selecting the nodes in next level. e.g. bits 0-7 are used to select among the 256 children of root, bits 8-15 are used to select among the children of next level.

The data structure is designed as follow:

typedef struct Trie trie;

typedef union Content content;

struct Trie{
  content content_;
}

union Content{
  int value_;
  trie ** children;
}

There will be 5 functions designed (pseudocode) as follows:
construct_trie_( ) : constructor function
destruct_trie_( Trie * ) : destruct all dynamically created variables
insert_element_( Trie *, index, value ) : update the value at the provided index
element_at_( Trie *, index ) : return the value at the provided index
iterate( Trie * ) : iterate through the sparse array and print the elements

============
