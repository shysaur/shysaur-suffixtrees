#include "sfxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


treenode_t *ukkonen(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  root->suffix_link = root;
  
  treenode_t *active_node = root;
  
  int j=0;
  /* i: character read at this stage + 1 */
  for (int i=1; i<=s.end; i++) {
    range_t cs = {0, i};
    
    /* j: starting character of the suffix we are appending str[i] to */
    for (; j<i; j++) {
      range_t csfx = {j, cs.end};
      
      splitpoint_t headsp;
    
      /* adjust active_node to be exactly at the head */
      range_t partial_head = active_node->node_val;
      int rest_end = RANGE_LEN(partial_head) + j;
      range_t rest = {rest_end, cs.end};
      headsp = slowscan(str, &rest, active_node);
            
      /* rule 3: is {j, i} in the tree? */
      if (headsp.arcpos + RANGE_LEN(headsp.parent->node_val) == RANGE_LEN(csfx)) {
        /* yes: fast forward rule 3 for all other j's and increment i */
        break;
      }
              
      /* rule 2: {j, i} not in the tree; do the split and add it */
      active_node = splitatpoint(&headsp);
    
      int head_end = j + RANGE_LEN(active_node->node_val);
      range_t ss = {j, INT_MAX};
      newchild(active_node, &ss);
      
      /* active node is the point of the last insertion */
      if (active_node->suffix_link == NULL) {
        splitpoint_t prefspl;
      
        /* set the suffix link searching for {j, head_end}. use the suffix
         * link of the parent to speed it up */
        range_t prefix_nexthead = {j+1, head_end};
        treenode_t *pfx_nexth_parent = active_node->parent->suffix_link;
        range_t r = pfx_nexth_parent->node_val;
        prefix_nexthead.start += RANGE_LEN(r);
        prefspl = fastscan(str, &prefix_nexthead, pfx_nexth_parent);
      
        treenode_t *linkt = splitatpoint(&prefspl);
        active_node->suffix_link = linkt;
      }
    
      active_node = active_node->suffix_link;
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

