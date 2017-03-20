#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  int start, end;   /* end not included */
} range_t;


typedef struct treenode_s {
  range_t arc_val;    /* substr of *arc* which led here. start==end==0 for root */
  range_t node_val;   /* substr of *path* which led here. start==end==0 for root */
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


void printrange(const char *str, range_t sfx)
{
  for (int i=sfx.start; i<sfx.end; i++)
    putchar(str[i] == '\0' ? '$' : str[i]);
}


/* Search where the longest prefix of ss (head(ss)) is located in the tree. 
 * McCreight calls this function "scanning" */
splitpoint_t slowscan(const char *str, const range_t *ss, treenode_t *tree)
{
  range_t sfx = *ss;
  splitpoint_t pos = {tree, NULL, 0};
  treenode_t *cur_child = tree;
  range_t arc = {0, 0};
  
  while (arc.start == arc.end && sfx.start < sfx.end) {
    tree = cur_child;
    
    cur_child = tree->first_child;
    while (cur_child) {
      arc = cur_child->arc_val;
      if (str[sfx.start] == str[arc.start])
        break;
      cur_child = cur_child -> next_sibling;
    }
      
    if (!cur_child) {
      pos.parent = tree;
      pos.child = NULL;
      pos.arcpos = 0;
      return pos;
    }
    
    pos.parent = tree;
    pos.child = cur_child;
    pos.arcpos = 0;
    while (arc.start < arc.end && sfx.start < sfx.end && 
           str[arc.start] == str[sfx.start]) {
      arc.start++;
      sfx.start++;
      pos.arcpos++;
    }
  }
  
  if (arc.start == arc.end) {
    pos.parent = pos.child;
    pos.child = NULL;
    pos.arcpos = 0;
  }
  return pos;
}


treenode_t *splitatpoint(const splitpoint_t *pos)
{
  if (pos->arcpos == 0)
    return pos->parent;
  
  treenode_t *new = calloc(1, sizeof(treenode_t));
  
  if (pos->child->prev_sibling == NULL)
    pos->parent->first_child = new;
  else
    pos->child->prev_sibling->next_sibling = new;

  if (pos->child->next_sibling)
    pos->child->next_sibling->prev_sibling = new;
  new->prev_sibling = pos->child->prev_sibling;
  new->next_sibling = pos->child->next_sibling;
  
  new->first_child = pos->child;
  pos->child->next_sibling = NULL;
  pos->child->prev_sibling = NULL;
  
  new->parent = pos->parent;
  pos->child->parent = new;
  
  new->arc_val.start = pos->child->arc_val.start;
  new->arc_val.end = new->arc_val.start + pos->arcpos;
  pos->child->arc_val.start = new->arc_val.end;
  new->node_val.start = pos->child->node_val.start;
  new->node_val.end = new->arc_val.end;
  
  return new;
}


treenode_t *newchild(treenode_t *parent, const range_t *node_val)
{
  treenode_t *new = calloc(1, sizeof(treenode_t));
  
  new->next_sibling = parent->first_child;
  if (parent->first_child)
    parent->first_child->prev_sibling = new;
  parent->first_child = new;
  new->parent = parent;
  
  new->node_val = *node_val;
  new->arc_val.end = node_val->end;
  new->arc_val.start = node_val->start + (parent->node_val.end - parent->node_val.start);
  
  return new;
}


treenode_t *sfxtree(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  newchild(root, &s);
  
  for (int i=1; i<s.end; i++) {
    range_t thispfx = {i, s.end};
    splitpoint_t split = slowscan(str, &thispfx, root);
    
    treenode_t *n = splitatpoint(&split);
    treenode_t *m = newchild(n, &thispfx);
  }
  
  return root;
}


void _printtree(const char *str, const treenode_t *root)
{
  printf("L%p [label=\"\" shape=point];\n", root);
  if (root->parent) {
    printf("L%p -> L%p [label=\"'", root->parent, root);
    printrange(str, root->arc_val);
    printf("'\"];\n");
  }
  if (root->first_child) {
    _printtree(str, root->first_child);
  }
  if (root->next_sibling) {
    _printtree(str, root->next_sibling);
  }
}


void printtree(const char *str, const treenode_t *root)
{
  printf("digraph G {\n");
  _printtree(str, root);
  printf("}\n");
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


