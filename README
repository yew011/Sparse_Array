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
=== just initialize everything to NULL.

destruct_trie_( Trie * ) : destruct all dynamically created variables
=== post order traversal and free the dynamically allocated memory.

insert_element_( Trie *, index, value ) : update the value at the provided index
=== check the passed in parameters
=== check from the root (level 0) to the corresponding leaf. if any node is not
=== created, create it (malloc). if the node has no children, create 256 children
=== and assign the pointer to the pointer (trie ** children).

element_at_( Trie *, index ) : return the value at the provided index
=== check the exsitance of element, if existed, return the value, otherwise, return -1

iterate( Trie * ) : iterate through the sparse array and print the elements
=== iterate through the sparse array and only print those values that are non-default

============
