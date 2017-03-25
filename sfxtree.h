#include <limits.h>


#ifndef __SFXTREE_H__
#define __SFXTREE_H__


typedef struct {
  int start, end;   /* end not included; end == INT_MAX => end of string */
} range_t;


typedef struct treenode_s {
  range_t arc_val;    /* substr of *arc* which led here. start==end==0 for root */
  range_t node_val;   /* substr of *path* which led here. start==end==0 for root */
  struct treenode_s *suffix_link;
  struct treenode_s *parent;
  struct treenode_s *next_sibling;
  struct treenode_s *prev_sibling;
  struct treenode_s *first_child;
} treenode_t;


typedef struct {
  treenode_t *parent;
  treenode_t *child;  /* can be NULL if arcpos == 0 */
  int arcpos;
} splitpoint_t;


/* length of a range_t */
#define RANGE_LEN(r) (((r)).end - ((r)).start)
/* length of a splitpoint_t */
#define POINT_LEN(r) (((r)).arcpos + RANGE_LEN(((r)).parent->node_val))

void printrange(const char *str, range_t sfx);
void printtree(const char *str, const treenode_t *root);

splitpoint_t fastscan(const char *str, const range_t *ss, treenode_t *tree);
splitpoint_t slowscan(const char *str, const range_t *ss, treenode_t *tree);
treenode_t *splitatpoint(const splitpoint_t *pos);
treenode_t *newchild(treenode_t *parent, const range_t *node_val);


#endif


