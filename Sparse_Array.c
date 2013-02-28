#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define CHILDREN_ 256
#define LEVEL_    4
#define IDX_BITS  8

// declare a node in trie.
// assume the trie has 4 levels.
typedef struct Trie    trie;
typedef union  Content content;

union Content{
  trie ** children_;
  int value_;
};

struct Trie{
  content content_;
};  

// the constructor function.
trie * construct_trie_(){
  trie * rt_ = (trie *)malloc( sizeof(trie) );
  if( rt_ == NULL ){
    fprintf( stderr, "Error: malloc in constructor.\n" );
    exit( -1 );
  }
  (rt_->content_).children_ = NULL;
  return rt_;
}

// the destructor function.
void destruct_trie_( trie * node_ ){
  if( node_ == NULL ) return;
  int i = 0;
  if( (node_->content_).children_ != NULL ){
    for( i=0; i<CHILDREN_; i++ )
      if( (node_->content_).children_[i] != NULL )
	destruct_trie_( (node_->content_).children_[i] );
  }
  free( node_ );
  return;
}

// the insert function.
int insert_( trie * root_, unsigned int idx_, int value_ ){
  if( root_ == NULL ) return -1;
  trie * tmp_ = root_;
  int i = 0;
  for( i=0; i<LEVEL_; i++ ){
    if( (tmp_->content_).children_ == NULL ){
      (tmp_->content_).children_ = (trie **)malloc( sizeof(trie*)*CHILDREN_ );
      for( i=0; i<CHILDREN_; i++ )
	(tmp_->content_).children_[i] = NULL;
    }
    if( (tmp_ = (tmp_->content_).children_[(idx_>>(IDX_BITS*i))&0xFF]) == NULL ){
      tmp_ = (trie *)malloc( sizeof(trie) );
      (tmp_->content_).children_ == NULL;
    }
  }
  (tmp_->content_).value_ = value_;
  return 0;
}

// the get function.
int get_( trie * root_, unsigned int idx_ ){
  trie * tmp_ = root_;
  if( tmp_ == NULL || 
      (tmp_->content_).children_ == NULL ||
      (tmp_ = (tmp_->content_).children_[idx_&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>8)&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>16)&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>24)&0xFF]) == NULL )
    return 0;
  return (tmp_->content_).value_;
}

// the iterate function.
void iterate_print_( trie * node_, unsigned int count_ ){
  if( count_ == CHILDREN_-1 && node_ != NULL )
    fprintf( stdout, "%d ", (node_->content_).value_ );
  else if( node_ != NULL && (node_->content_).children_ != NULL )
    iterate_print_( node_, count_++ );
}

int iterate_( trie * root_ ){
  if( root_ == NULL || (root_->content_).children_ == NULL )
    return -1;
  iterate_print_( root_, (unsigned int) 0 );
  fprintf( stdout, "\n" );
  return 0;
}

// main function, for testing.
int main( int argc, char ** argv ){
  trie * test_trie_ = NULL;

  /* test cases:
     Tests repeated for empty trie_, trie_ only least 8 bits filled, trie_ sparsely filled.
     1. Test the creation of trie_.
     2. Test the insert of trie_.
     3. Test the get of trie_.
     4. Test the iterate of trie_.
     5. Test the destructor of trie_.
   */

  // Test for empty trie_.
  test_trie_ = construct_trie_();
  assert( test_trie_ != NULL );
  assert( (test_trie_->content_).children_ == NULL );
  assert( get_(test_trie_, 1024) == 0 );
  destruct_trie_(test_trie_);
  fprintf( stdout, "Success: Test Create/Delete Empty Spare Array.\n" );

  return 0;
}
