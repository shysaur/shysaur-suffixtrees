#include "sfxtree.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 
 * Primary reference: McCreight, Edward M. "A space-economical suffix tree 
 * construction algorithm." Journal of the ACM (JACM) 23.2 (1976): 262-272. 
 * 
 * The names of most variables are consistent with the terminology used by
 * McCreight.
 */


treenode_t *suffixTree_mcCreight(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  root->suffix_link = root;
  
  newChild(root, &s);
  
  /* Loop invariant: active_node must represent a prefix of the current
   * suffix */
  treenode_t *active_node = root;
  
  /* Suffixes are inserted in order from the longest to the shortest. */
  for (int i=1; i<s.end; i++) {
    range_t suf = {i, s.end};  /* suffix that will be inserted */
    
    /* Search for the current 'head' by scanning until we reach the
     * first character that does not match or we "fall out" of the tree.
     * The 'head' is defined as the longest prefix of the current suffix
     * that is already present in the tree as a prefix of another suffix. 
     * 'tail' is the rest of the suffix (the part after the head).
     * gamma is the last part of 'head' that has to be scanned. */
    range_t partial_head = active_node->node_val;
    int gamma_start = RANGE_LEN(partial_head) + i;
    range_t gammatail = {gamma_start, s.end};
    treepoint_t start = {active_node, NULL, 0};
    treepoint_t head = slowScan(str, &gammatail, &start);
    
    /* Split where 'head' ends and insert the new suffix */
    active_node = splitAtPoint(&head);
    newChild(active_node, &suf);
    
    /* Set the suffix link of the head node, if needed */
    if (active_node->suffix_link == NULL) {
      treepoint_t prefspl;
            
      /* Search the location in the tree of the current head without the
       * first character. Use the suffix link of the parent to speed it up. */
      range_t csialphabeta = active_node->node_val;
      /* alphabeta: current head except the first character (csi).
       * The case where csi is empty is implicitly handled. */
      range_t alphabeta = {csialphabeta.start+1, csialphabeta.end};
      /* beta: portion of head on the last arc in the path to head
       * alpha: alphabeta without beta */
      treenode_t *alpha_node = active_node->parent->suffix_link;
      range_t alpha = alpha_node->node_val;
      range_t beta = {alphabeta.start + RANGE_LEN(alpha), alphabeta.end};
      treepoint_t alphabeta_point = fastScan(str, &beta, alpha_node);
      
      treenode_t *linkt = splitAtPoint(&alphabeta_point);
      active_node->suffix_link = linkt;
    }
    
    /* Lemma 1 in McCreight's paper states that by removing the 1st character
     * from the current prefix's head, we get a prefix of the next suffix's
     * head. Thus we can use the suffix link to speed up the search for
     * the next head. */
    active_node = active_node->suffix_link;
  }
  
  return root;
}


int main(int argc, char *argv[])
{
  if (argc < 2) {
    benchmark(suffixTree_mcCreight);
  } else {
    char *str = argv[1];
    treenode_t *t = suffixTree_mcCreight(str);
    printTree(str, t);
  }
  return 0;
}


