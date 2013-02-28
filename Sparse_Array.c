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
trie * construt_trie_(){
  trie * rt_ = (trie *)malloc( sizeof(trie) );
  if( rt_ == NULL ){
    fprintf( stderr, "Error: malloc in constructor.\n" );
    exit( -1 );
  } 
  (rt_->content_).children_ == NULL;
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
int insert( trie * root_, unsigned int idx_, int value_ ){
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
int get( trie * root_, unsigned int idx_ ){
  trie * tmp_ = root_;
  if( tmp_ == NULL || 
      (tmp_ = (tmp_->content_).children_[idx_&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>8)&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>16)&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>24)&0xFF]) == NULL )
    return 0;
  return (tmp_->content_).value_;
}

// the iterate function.
int iterate_( trie * trie_ ){
  return 0;
}

// main function.
int main( int argc, char ** argv ){

  return 0;
}
