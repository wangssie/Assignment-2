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
#include <string.h>
#include <math.h>
#define RESIZE_FACTOR 2
#define DEFAULT_EDGE_ARRAY_SIZE 2 // must always be less than resize factor
#define MAX_EDGES 27
#define END_SYMBOL '$'
#define START_SYMBOL '^'
#define NULL_CHAR '\0'


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
  node_t *node = (node_t*)malloc(sizeof(node_t));
  assert(node!=NULL);

  node->c = c;
  node->freq = 1;
  node->edgeArray = edgeArray;
  node->prevNode = prevNode;
  node->edgeCount = 0;
  node->depth = prevNode->depth+1;
  return node;
}

// Create the head node of the trie, character ^
node_t *createHeadNode() {
  node_t **edgeArray = createEdgeArray();
  node_t *node = (node_t*)malloc(sizeof(node_t));
  assert(node!=NULL);

  node->c = START_SYMBOL;
  node->freq = 0;
  node->edgeArray = edgeArray;
  node->prevNode = NULL;
  node->edgeCount = 0;
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
  if (node->edgeCount == 0) {
    return -1;
  }
  return binarySearch(c, node->edgeArray, 0, node->edgeCount -1);
}

// Find which index the node with character c should be put in this edge array
// in order to maintain ordered nature
// AKA Find the smallest node that is greater than c
int binarySearchPosition(char c, node_t **edgeArray, int L, int R) {
  // L surpassed R, therfore all elements are less than c
  if (L>R) {
    return L;
  }
  int i = (L+R)/2;
  int compare = compareChar(c, (*(edgeArray+i))->c);
  // smallest node greater than c is found, return index
  if (L==R && compare<0) {
    return L;
  }
  // choose middle index in array
  // if character is less that node at index i
  if (compare<0) {
    // continue search below i, including i
    return binarySearchPosition(c, edgeArray, L, i);
  }
  // if character is greater than node at index i
  else {
    // continue search beyond node at index i
    return binarySearchPosition(c, edgeArray, i+1, R);
  }
}

int findEdgeArrayPosition(node_t *addedNode, node_t *prevNode) {
  // if edge array is empty
  if (prevNode->edgeCount ==0) {
    return 0;
  }
  else {
    return binarySearchPosition(addedNode->c, prevNode->edgeArray, 0, prevNode->edgeCount-1);
  }
}

// Resize the edge array if it has reached full capacity
// Increase the size by factor until it will exceed max number of edges
void resizeEdgeArray(node_t *node) {
  int i, bound, possibleBound;
  for (i=1; pow(RESIZE_FACTOR,i)<=MAX_EDGES; i++) {
    bound = pow(RESIZE_FACTOR, i);
    // has yet to reach edgeArray full capacity
    if (node->edgeCount < bound) {
      return;
    }
    // if full capacity reached
    if (node->edgeCount == bound) {
      // DEBUG printf("array of node %p is resized\n", node);
      // if multiplying by factor+1 will exceed max number of edges
      possibleBound = pow(RESIZE_FACTOR, i+1);
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
void addEdge(node_t *addedNode, node_t *prevNode) {
  // DEBUG printf("adding %c (%p) to %c's (%p) edgeArray\n", addedNode->c, addedNode, prevNode->c, prevNode);
  // search position
  int index = findEdgeArrayPosition(addedNode, prevNode);
  // DEBUG printf("position in edge array: %d\n", index);
  // resize array
  resizeEdgeArray(prevNode);
  // shift array one position down
  int i;
  for (i=(prevNode->edgeCount)-1; i>=index; i--) {
    *((prevNode->edgeArray)+i+1)=*((prevNode->edgeArray)+i);
    // DEBUG printf("moving node %c (%p) one position up\n",(*((prevNode->edgeArray)+i))->c,*((prevNode->edgeArray)+i));
  }
  // place node in position
  *((prevNode->edgeArray)+index) = addedNode;
  prevNode -> edgeCount ++;
  // DEBUG
  /* printf("edge array of %c (%p):[", prevNode->c, prevNode);
  for (i=0; i<prevNode->edgeCount; i++) {
    printf("%c ", (*((prevNode->edgeArray)+i)) -> c);
  }
  printf("]\n");*/

}

// Preorder traversal through trie and printing each traverse node's character
void traversePrint(node_t *node) {
  printf("%c\n", node->c, node->depth, node->freq);
  int i;
  for (i=0; i<node->edgeCount; i++) {
    traversePrint(*((node->edgeArray)+i));
  }
}

// Add end node to the end of the word pathway in trie
void addEndNode(node_t *lastNode) {
  // create the end node
  node_t *endNode = createNode(END_SYMBOL, lastNode);
  addEdge(endNode, lastNode); // could implement better function where it doesn't binary search but always adds it to the start of the array
}

char *appendEndSymbol(char *word) {
  int len = strlen(word);
  word = (char*)realloc(word, (len+2)*sizeof(char));
  *(word+len+1) = NULL_CHAR;
  *(word+len) = END_SYMBOL;
  return word;
}

// Create a trie and return it's head node
node_t *createTrie(int N) {
  // create head node
  node_t *head = createHeadNode(), *currNode, *nextNode;
  int i, j, x, charIndex;
  char* word;
  for (i=0; i<N; i++) {
    // Get the next word to add to trie
    word = getWord();
    word = appendEndSymbol(word);
    // DEBUG printf("\n\nword: %s\n", word);
    currNode = head;
    // update frequency of total number words
    head->freq++;
    // iterate through each character in word
    for (j=0; j<strlen(word); j++) {
      // DEBUG printf("char: %c ", *(word+j));
      charIndex = searchEdgeArray(currNode, *(word+j));
      // if character node already exists in current node's edges
      if (charIndex != -1) {
        // DEBUG printf("FOUND\n");
        nextNode = *((currNode->edgeArray)+charIndex);
        nextNode->freq++;
        // DEBUG printf("freq of node %c (%p) at depth %d: %d\n",nextNode->c, nextNode, nextNode->depth,nextNode->freq);
        // traverse to the character node
        currNode = nextNode;
        // look for next character in word
        continue;
      }
      // if character is not found
      else {
        // DEBUG printf("NOT FOUND\n");
        // for remaining letters
        for (x=j; x<strlen(word); x++) {
          nextNode = createNode(*(word+x), currNode);
          // DEBUG printf("created new node: %c (%p), depth: %d\n", nextNode->c, nextNode, nextNode->depth );
          addEdge(nextNode, currNode);
          currNode = nextNode;
        }
        // add end symbol node to end of word in trie
        break;
      }
    }
  }
  return head;
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
  int N;
  scanf("%d\n", &N);

  node_t *trieHead = createTrie(N);

  traversePrint(trieHead);
  freeTrie(trieHead);
}

// Get the full string from the head trie to a specified node
char *getFullString(node_t *node) {
  node_t *currNode = node;
  int stringLength = node->depth, i;
  char* string = malloc((stringLength+1)*sizeof(char));

  // copy path of node's characters to string
  for (i=stringLength-1; i>=0; i--) {
    *(string+i) = currNode->c;
    currNode = currNode->prevNode;
  }
  // add null character at the end
  *(string+stringLength) = '\0';

  return string;
}

// Traverse preorderly through trie and print prefixes (and their frequencies) with length K
void traverseK(node_t *node, int K) {
  char *string;
  int i;
  // if length of path is equal to K and node is not an end symbol
  if (K==node->depth && node->c != END_SYMBOL) {
    string = getFullString(node);
    printf("%s %d\n", string, node->freq);
    free(string);
  }
  else if (node->edgeCount != 0) {
    for (i=0; i<node->edgeCount; i++) {
      traverseK(*((node->edgeArray)+i), K);
    }
  }
  return;
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
  int N, K;
  scanf("%d %d\n", &N, &K);

  node_t *trieHead = createTrie(N);

  traverseK(trieHead, K);

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
