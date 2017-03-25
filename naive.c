#include "sfxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


treenode_t *sfxtree(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  newchild(root, &s);
  
  for (int i=1; i<s.end; i++) {
    range_t thispfx = {i, s.end};
    splitpoint_t start = {root, NULL, 0};
    splitpoint_t split = slowscan(str, &thispfx, &start);
    
    treenode_t *n = splitatpoint(&split);
    treenode_t *m = newchild(n, &thispfx);
  }
  
  return root;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return 1;
  char *str = argv[1];
  treenode_t *t = sfxtree(str);
  printtree(str, t);
  return 0;
}


