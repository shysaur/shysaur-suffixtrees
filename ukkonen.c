#include "sfxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


treenode_t *ukkonen(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  root->suffix_link = root;
  
  /* loop invariant: active_pos must be the position in the tree where the
   * preexisting suffix we want to extend ends */
  splitpoint_t active_pos = {root, NULL, 0};
  
  int j=0;
  /* i: character read at this stage + 1 */
  for (int i=1; i<=s.end; i++) {
    range_t cs = {0, i};
    
    /* j: starting character of the suffix we are appending str[i-1] to */
    for (; j<i; j++) {
      range_t csfx = {j, cs.end};
    
      /* rule 3: is {j, i} in the tree? */
      range_t rest = {i-1, i};
      active_pos = slowscan(str, &rest, &active_pos);
      
      if (POINT_LEN(active_pos) == RANGE_LEN(csfx)) {
        /* yes: fast forward rule 3 for all other j's and increment i 
         * we will restart at this same suffix (all other longer suffixes - 
         * rule 1 - are already inserted because of the unbounded ranges trick),
         * but it will be one character longer; thus the active_pos invariant
         * is already taken care of */
        break;
      }
              
      /* rule 2: {j, i} not in the tree; do the split and add it */
      active_pos.parent = splitatpoint(&active_pos);
      range_t ss = {j, INT_MAX};
      newchild(active_pos.parent, &ss);
      
      /* active node is the point of the last insertion */
      if (active_pos.parent->suffix_link == NULL) {
        splitpoint_t prefspl;
      
        /* set the suffix link searching for {j+1, i-1}. use the suffix
         * link of the parent to speed it up */
        range_t prefix_nexthead = {j+1, i-1};
        treenode_t *pfx_nexth_parent = active_pos.parent->parent->suffix_link;
        range_t r = pfx_nexth_parent->node_val;
        prefix_nexthead.start += RANGE_LEN(r);
        prefspl = fastscan(str, &prefix_nexthead, pfx_nexth_parent);
      
        treenode_t *linkt = splitatpoint(&prefspl);
        active_pos.parent->suffix_link = linkt;
      }
        
      active_pos.parent = active_pos.parent->suffix_link;
      active_pos.child = NULL;
      active_pos.arcpos = 0;
    }
  }

  return root;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return 1;
  char *str = argv[1];
  treenode_t *t = ukkonen(str);
  printtree(str, t);
  return 0;
}

