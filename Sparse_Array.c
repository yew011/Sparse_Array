#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

/* Constant declaration.*/
#define CHILDREN_ 256
#define LEVEL_    4
#define IDX_BITS  8

/* Trie structure.*/
struct Trie {
  union Content {
    struct Trie **children_;
    int value_;
  } content_;
};  

// the constructor function.
struct Trie * construct_trie_()
{
  struct Trie *rt_ = malloc(sizeof(struct Trie));
  if (rt_ == NULL) {
    fprintf(stderr, "Error: malloc in constructor.\n");
    exit(-1);
  }
  (rt_->content_).children_ = NULL;
  return rt_;
}

// the destructor function.
void destruct_trie_(struct Trie *node_, int level_)
{
  int i;
  if (node_ == NULL) {
    return;
  }
  if (level_ < LEVEL_
      && (node_->content_).children_ != NULL) {
    for (i = 0; i < CHILDREN_; i++) {
      if ((node_->content_).children_[i] != NULL) {
	destruct_trie_((node_->content_).children_[i], level_+1);
      }
    }
  }
  free(node_);
  return;
}

/* the insert function.*/
int insert_(struct Trie *root_, unsigned int idx_, int value_)
{
  struct Trie *tmp_ = root_;
  int i;
  if (root_ == NULL) {
    return -1;
  }
  for (i = 0; i < LEVEL_; i++) {
    if ((tmp_->content_).children_ == NULL) {
      (tmp_->content_).children_ = malloc(sizeof(struct Trie*)*CHILDREN_);
      memset((tmp_->content_).children_, 0, sizeof(struct Trie*)*CHILDREN_);
    }
    if (((tmp_->content_).children_[(idx_>>(IDX_BITS*i))&0xFF]) == NULL) {
      (tmp_->content_).children_[(idx_>>(IDX_BITS*i))&0xFF] = malloc(sizeof(struct Trie));
      tmp_ = (tmp_->content_).children_[(idx_>>(IDX_BITS*i))&0xFF];
      (tmp_->content_).children_ = NULL;
    } else {
      tmp_ = (tmp_->content_).children_[(idx_>>(IDX_BITS*i))&0xFF];
    }
  }
  (tmp_->content_).value_ = value_;
  return 0;
}

/* the get function.*/
int get_(struct Trie *root_, unsigned int idx_)
{
  struct Trie *tmp_ = root_;
  if ((tmp_ == NULL) ||
      (tmp_->content_).children_ == NULL ||
      (tmp_ = (tmp_->content_).children_[idx_&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>8)&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>16)&0xFF]) == NULL ||
      (tmp_ = (tmp_->content_).children_[(idx_>>24)&0xFF]) == NULL) {
    fprintf(stderr, "Error: node not exist, in get_.\n");
    return 0x80000000;
  }
  return (tmp_->content_).value_;
}

void iterate_print_(struct Trie *node_, int level_)
{
  int i;
  if (level_ == LEVEL_) {
    fprintf(stdout, "%X ", (node_->content_).value_);
  } else {
    if ((node_->content_).children_ != NULL) {
      for (i = 0; i < CHILDREN_; i++) {
	if ((node_->content_).children_[i] != NULL) {
	  iterate_print_((node_->content_).children_[i], level_+1);
	}
      }
    }
  }
}
  /* the iterate function.*/
int iterate_(struct Trie *root_)
{
  if (root_ == NULL || (root_->content_).children_ == NULL) {
    fprintf(stderr, "Error: uninitialized trie in iterate_.\n");
    return -1;
  }
  iterate_print_(root_, 0);
  fprintf(stdout, "\n");
  return 0;
}

// main function, for testing.
int main(int argc, char **argv)
{
  struct Trie *test_trie_ = NULL;
  /* test cases:
     Tests repeated for empty trie_, trie_ with two ends filled, trie_ only least 8 bits filled, trie_ sparsely filled.
     1. Test the creation of trie_.
     2. Test the insert of trie_.
     3. Test the get of trie_.
     4. Test the iterate of trie_.
     5. Test the destructor of trie_.
   */

  /* Test for empty trie_.*/
  fprintf(stdout, "[Start]: Test Create/Delete Empty Sparse Array.\n");
  test_trie_ = construct_trie_();
  assert(test_trie_ != NULL);
  assert((test_trie_->content_).children_ == NULL);
  fprintf(stdout, "   INVOKE: the get_ function for empty trie_.\n");
  fprintf(stderr, "   INTENDED OUTPUT: ");
  assert(get_(test_trie_, 1024) == 0x80000000);
  fprintf(stdout, "   INVOKE: the iterate_ function.\n");
  fprintf(stderr, "   INTENDED OUTPUT: ");
  iterate_(test_trie_);
  destruct_trie_(test_trie_, 0);
  test_trie_ = NULL;
  fprintf(stdout, "[Success]: Test Create/Delete Empty Sparse Array.\n");

  /* Test for trie_ with only least 8 bits filled.*/
  fprintf(stdout, "[Start]: Test Sparse Array With Two Ends Filled.\n");
  test_trie_ = construct_trie_();
  insert_(test_trie_, 0, 0);
  assert(get_(test_trie_, 0) == 0);
  assert(~((unsigned int)0) == (unsigned int)0 - (unsigned int)1);
  insert_(test_trie_, ~((unsigned int)0), ~((unsigned int)0));
  assert(get_(test_trie_, ~((unsigned int)0)) == ~((unsigned int)0));
  fprintf(stdout, "   INVOKE: the iterate_ function.\n");
  fprintf(stdout, "   EXPECTED OUTPUT: 0, -1.\n");
  fprintf(stdout, "   OUPUT: ");
  iterate_(test_trie_);
  destruct_trie_(test_trie_, 0);
  test_trie_ = NULL;
  fprintf(stdout, "[Success]: Test Spare Array With Two Ends Filled.\n");

  /* Test for least 8 bits filled.*/
  fprintf(stdout, "[Start]: Test Sparse Array With Least 8-bits Filled.\n");
  test_trie_ = construct_trie_();
  int i;
  for (i = 0; i <= 0xFF; i++) {
    insert_(test_trie_, i, i);
  }
  for (i = 0; i <= 0xFF; i++) {
    assert(get_(test_trie_, i) == i);
  }
  fprintf(stdout, "   INVOKE: the iterate_ function.\n");
  fprintf(stdout, "   EXPECTED OUTPUT: 0 - 255.\n");
  fprintf(stdout, "   OUPUT: ");
  iterate_(test_trie_);
  destruct_trie_(test_trie_, 0);
  test_trie_ = NULL;
  fprintf(stdout, "[Success]: Test Sparse Array With Least 8-bits Filled.\n");

  /* Test for a real sparse array.*/
  fprintf(stdout, "[Start]: Test A Real Sparse Array of Size 100.\n");
  // create 100 random numbers and keep record of those 100 numbers
  int j;
  int arr_[100];
  for (i = 0; i < 100; i++) {
    arr_[i] = 0;
    for (j = 0; j < 10; j++) {
      arr_[i] *= 10;
      arr_[i] += rand()%10;
    }
  }
  test_trie_ = construct_trie_();
  for (i = 0; i < 100; i++) {
    insert_(test_trie_, arr_[i], arr_[i]);
  }
  for (i = 0; i < 100; i++) {
    assert(get_(test_trie_, arr_[i]) == arr_[i]);
  }
  fprintf(stdout, "   INVOKE: the iterate_ function.\n");
  fprintf(stdout, "   EXPECTED OUTPUT: ");
  for (i = 0; i < 100; i++) {
    fprintf(stdout, "%X ", arr_[i]);
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "   OUPUT: ");
  iterate_(test_trie_);
  destruct_trie_(test_trie_, 0);
  test_trie_ = NULL;
  fprintf(stdout, "[Success]: Test A Real Sparse Array of Size 100.\n");

  fprintf(stdout, "[ALL TEST FINISHED].\n");
  return 0;
}
