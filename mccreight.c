#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  int start, end;   /* end not included */
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


void printrange(const char *str, range_t sfx)
{
  for (int i=sfx.start; i<sfx.end; i++)
    putchar(str[i] == '\0' ? '$' : str[i]);
}


/* Search where, in the tree, the string sfx appears. It must be
 * present in the tree in its entirety (not necessarily at a leaf node) 
 * McCreight calls this function "rescanning" */
void fastscan(const char *str, range_t sfx, treenode_t *tree, splitpoint_t *pos)
{
  range_t arc = {0, 0};
  treenode_t *cur_child = tree;
  
  while (sfx.end > sfx.start) {
    tree = cur_child;
    
    cur_child = tree->first_child;
    while (cur_child) {
      arc = cur_child->arc_val;
      if (str[sfx.start] == str[arc.start])
        break;
      cur_child = cur_child -> next_sibling;
    }
      
    if (!cur_child) {
      pos->parent = NULL;
      pos->child = NULL;
      pos->arcpos = 0;
      return;
    }
    
    sfx.start += arc.end - arc.start;
  }
  
  if (sfx.start - sfx.end != 0) {
    pos->parent = tree;
    pos->child = cur_child;
    pos->arcpos = (arc.end - arc.start) - (sfx.start - sfx.end);
  } else {
    pos->parent = cur_child;
    pos->child = NULL;
    pos->arcpos = 0;
  }
}


/* Search where the longest prefix of sfx (head(sfx)) is located in the tree. 
 * McCreight calls this function "scanning" */
void slowscan(const char *str, range_t sfx, treenode_t *tree, splitpoint_t *pos)
{
  treenode_t *cur_child = tree;
  pos->arcpos = 0;
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
      pos->parent = tree;
      pos->child = NULL;
      pos->arcpos = 0;
      return;
    }
    
    pos->parent = tree;
    pos->child = cur_child;
    pos->arcpos = 0;
    while (arc.start < arc.end && sfx.start < sfx.end && 
           str[arc.start] == str[sfx.start]) {
      arc.start++;
      sfx.start++;
      pos->arcpos++;
    }
  }
  
  if (arc.start == arc.end) {
    pos->parent = pos->child;
    pos->child = NULL;
    pos->arcpos = 0;
  }
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


treenode_t *newchild(treenode_t *parent, const range_t node_val)
{
  treenode_t *new = calloc(1, sizeof(treenode_t));
  
  new->next_sibling = parent->first_child;
  if (parent->first_child)
    parent->first_child->prev_sibling = new;
  parent->first_child = new;
  new->parent = parent;
  
  new->node_val = node_val;
  new->arc_val.end = node_val.end;
  new->arc_val.start = node_val.start + (parent->node_val.end - parent->node_val.start);
  
  return new;
}


treenode_t *mccreight(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  root->suffix_link = root;
  
  newchild(root, s);
  
  treenode_t *active_node = root;
  
  for (int i=1; i<s.end; i++) {
    splitpoint_t headsp;
    
    /* adjust active_node to be exactly at the head */
    range_t partial_head = active_node->node_val;
    int rest_end = partial_head.end - partial_head.start + i;
    range_t rest = {rest_end, s.end};
    slowscan(str, rest, active_node, &headsp);
    
    active_node = splitatpoint(&headsp);
    
    int head_end = active_node->node_val.end - active_node->node_val.start + i;
    range_t ss = {i, s.end};
    newchild(active_node, ss);
    
    if (active_node->suffix_link == NULL) {
      splitpoint_t prefspl;
      
      /* set the suffix link searching for {i+1, head_end}. use the suffix
       * link of the parent to speed it up */
      range_t prefix_nexthead = {i+1, head_end};
      treenode_t *pfx_nexth_parent = active_node->parent->suffix_link;
      range_t r = pfx_nexth_parent->node_val;
      prefix_nexthead.start += r.end - r.start;
      fastscan(str, prefix_nexthead, pfx_nexth_parent, &prefspl);
      
      treenode_t *linkt = splitatpoint(&prefspl);
      active_node->suffix_link = linkt;
    }
    
    active_node = active_node->suffix_link;
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
  if (root->suffix_link) {
    printf("L%p -> L%p [style=dotted];\n", root, root->suffix_link);
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
  treenode_t *t = mccreight(str);
  printtree(str, t);
  return 0;
}


