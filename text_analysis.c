/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Insert Name Here>
 */

#include "text_analysis.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define RESIZE_FACTOR 2
#define DEFAULT_EDGE_ARRAY_SIZE 2 // must always be less than resize factor
#define MAX_EDGES 27


// Compares two characters, c1 and c2 and returns:
  // 0 if they are equal
  // -1 if c1<c2
  // 1 if c1>c2
int compareChar(char c1, char c2) {
  if (c1==c2) {
    return 0;
  }
  else if ((int)c1<(int)c2 || c1 == '$') {
    return -1;
  }
  return 1;
}

// Create an array of nodes that keeps track of its children
node_t **createEdgeArray() {
  node_t **edgeArray = (node_t**)malloc(sizeof(node_t*)*DEFAULT_EDGE_ARRAY_SIZE);
  assert(edgeArray != NULL);
  return edgeArray;
}

// Create a node:
  // c <- character stored in node
  // depth <- the level the node is on in the trie
  // prevNode <- the parent of the node
node_t *createNode(char c, node_t *prevNode) {
  node_t **edgeArray = createEdgeArray();
  node_t *node;
  node->c = c;
  node->freq = 1;
  node->edgeArray = edgeArray;
  node->prevNode = prevNode;
  node->depth = prevNode->depth+1;
  return node;
}

// Create the head node of the trie, character ^
node_t *createHeadNode() {
  node_t **edgeArray = createEdgeArray();
  node_t *node;
  node->c = '^';
  node->freq = 0;
  node->edgeArray = edgeArray;
  node->prevNode = NULL;
  node->depth = 0;
  return node;
}

// Free the edge array of the node
void freeEdgeArray(node_t *node) {
  free(node->edgeArray);
}

// Free the node and its children
void freeNode(node_t *node) {
  int i;
  for (i=0; i<node->edgeCount; i++) {
    freeNode(*(node->edgeArray+i));
  }
  freeEdgeArray(node);
  free(node);
}

// Free the trie
void freeTrie(node_t *head) {
  freeNode(head);
}

// Perform binary search of character c in edge Array
int binarySearch(char c, node_t **edgeArray, int L, int R) {
  // character not found in array
  if (L>R) {
    return -1;
  }

  // middle index
  int i = (R+L)/2;
  int compare = compareChar(c, (*(edgeArray+i))->c);
  // found the character
  if (compare==0) {
    return i;
  }
  // character is less than node at index i
  else if (compare <0) {
    return binarySearch(c, edgeArray, L, i-1);
  }
  // character is greater than node at index i
  else {
    return binarySearch(c, edgeArray, i+1, R);
  }
}

// Search is character c exists as a child node for the node
// if found -> returns index of node in egde array
// if not found -> returns -1
int searchEdgeArray(node_t *node, char c) {
  return binarySearch(c, node->edgeArray, 0, node->edgeCount -1);
}

// Find which index the node with character c should be put in this edge array
// in order to maintain ordered nature
// AKA Find the smallest node that is greater than c
int searchIndexPosition(char c, node_t **edgeArray, int L, int R) {
  // smallest node greater than c is found, return index
  if (L==R) {
    return L;
  }
  // choose middle index in array
  int i = (L+R)/2;
  int compare = compareChar(c, (*(edgeArray+i))->c);
  // if character is less that node at index i
  if (compare<0) {
    // continue search below i, including i
    return searchIndexPosition(c, edgeArray, L, i);
  }
  // if character is greater than node at index i
  else {
    // continue search beyond node at index i
    return searchIndexPosition(c, edgeArray, i+1, R);
  }
}

// Resize the edge array if it has reached full capacity
// Increase the size by factor until it will exceed max number of edges
void resizeEdgeArray(node_t *node, int factor) {
  int i;
  for (i=1; pow(factor,i)<=MAX_EDGES; i++) {
    int bound = pow(factor, i);
    // has yet to reach edgeArray full capacity
    if (node->edgeCount < bound) {
      return;
    }
    // if full capacity reached
    if (node->edgeCount == bound) {
      // if multiplying by factor+1 will exceed max number of edges
      int possibleBound = pow(factor, i+1);
      if (possibleBound>MAX_EDGES) {
        // realloc with max number of edges possible
        node->edgeArray = (node_t**)realloc(node->edgeArray, sizeof(node_t*)*MAX_EDGES);
      }
      else {
        // increase array size by factor
        node->edgeArray = (node_t**)realloc(node->edgeArray, sizeof(node_t*)*possibleBound);
      }
    }
  }
}

// Add a character into the current node's array of child nodes
// Ensure that sorted nature of array is maintained
void addEdge(char c, node_t *prevNode) {
  // Create a new node for the character c
  node_t *addedNode = createNode(c, prevNode);

  // search position
  int index = searchIndexPosition(c, prevNode->edgeArray, 0, prevNode->edgeCount-1);
  // resize array
  resizeEdgeArray(prevNode, RESIZE_FACTOR);
  // shift array one position down
  int i;
  for (i=prevNode->edgeCount-1; i>=index; i--) {
    *((prevNode->edgeArray)+i+1)=*((prevNode->edgeArray)+i);
  }
  // place node in position
  *((prevNode->edgeArray)+index) = addedNode;
}

// Preorder traversal through trie and printing each traverse node's character
void traversePrint(node_t *node) {
  printf("%c\n", node->c);
  int i;
  for (i=0; i<node->edgeCount; i++) {
    traversePrint(*((node->edgeArray)+i));
  }
}



// Build a character level trie for a given set of words.
//
// The input to your program is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Your program should built a character level trie where each node indicates
// a single character. Branches should be ordered in alphabetic order.
//
// Your program must output the pre-order traversal of the characters in
// the trie, on a single line.
void problem_2_a() {
  // TODO: Implement Me!
}

// Using the trie constructed in Part (a) this program should output all
// prefixes of length K, in alphabetic order along with their frequencies
// with their frequencies. The input will be:
//   n k
//   str_0
//   ...
//   str_(n-1)
// The output format should be as follows:
//   an 3
//   az 1
//   ba 12
//   ...
//   ye 1
void problem_2_b() {
  // TODO: Implement Me!
}

// Again using the trie data structure you implemented for Part (a) you will
// provide a list (up to 5) of the most probable word completions for a given
// word stub.
//
// For example if the word stub is "al" your program may output:
//   0.50 algorithm
//   0.25 algebra
//   0.13 alright
//   0.06 albert
//   0.03 albania
//
// The probabilities should be formatted to exactly 2 decimal places and
// should be computed according to the following formula, for a word W with the
// prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to your program will be the following:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
// That is, there are n + 1 strings in total, with the first being the word
// stub.
//
// If there are two strings with the same probability ties should be broken
// alphabetically (with "a" coming before "aa").
void problem_2_c() {
  // TODO: Implement Me!
}
