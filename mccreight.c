#include "sfxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


treenode_t *mccreight(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  root->suffix_link = root;
  
  newchild(root, &s);
  
  treenode_t *active_node = root;
  
  for (int i=1; i<s.end; i++) {
    splitpoint_t headsp;
    
    /* adjust active_node to be exactly at the head */
    range_t partial_head = active_node->node_val;
    int rest_end = RANGE_LEN(partial_head) + i;
    range_t rest = {rest_end, s.end};
    headsp = slowscan(str, &rest, active_node);
    
    active_node = splitatpoint(&headsp);
    
    int head_end = RANGE_LEN(active_node->node_val) + i;
    range_t ss = {i, s.end};
    newchild(active_node, &ss);
    
    if (active_node->suffix_link == NULL) {
      splitpoint_t prefspl;
      
      /* set the suffix link searching for {i+1, head_end}. use the suffix
       * link of the parent to speed it up */
      range_t prefix_nexthead = {i+1, head_end};
      treenode_t *pfx_nexth_parent = active_node->parent->suffix_link;
      range_t r = pfx_nexth_parent->node_val;
      prefix_nexthead.start += RANGE_LEN(r);
      prefspl = fastscan(str, &prefix_nexthead, pfx_nexth_parent);
      
      treenode_t *linkt = splitatpoint(&prefspl);
      active_node->suffix_link = linkt;
    }
    
    active_node = active_node->suffix_link;
  }
  
  return root;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return 1;
  char *str = argv[1];
  treenode_t *t = mccreight(str);
  printtree(str, t);
  return 0;
}


