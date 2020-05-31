/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Serena Wang
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
#define P2C_COMPLETIONS 5


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

// Create an array of nodes that keeps track of a certain node's children
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

// Create the head node of the trie, character is the START_SYMBOL (^)
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

// Free the trie and all the nodes in the trie
void freeTrie(node_t *head) {
  freeNode(head);
}

// Performs binary search of character c in edge erray
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

// Checks if character c exists as a child node for the node
// if yes -> returns index of node in egde array
// if not -> returns -1
int searchEdgeArray(node_t *node, char c) {
  if (node->edgeCount == 0) {
    return -1;
  }
  return binarySearch(c, node->edgeArray, 0, node->edgeCount -1);
}

// Binary search function that helps findEdgeArrayPosition function
int binarySearchPosition(char c, node_t **edgeArray, int L, int R) {
  // L surpassed R, therfore all elements are less than c
  if (L>R) {
    return L;
  }
  // choose middle index in array
  int i = (L+R)/2;
  int compare = compareChar(c, (*(edgeArray+i))->c);
  // one element left and is greater than c
  if (L==R && compare<0) {
    return L;
  }
  // if character is less that node at index i
  if (compare<0) {
    // continue search below i, including i
    return binarySearchPosition(c, edgeArray, L, i);
  }
  // if character is greater than node at index i
  else {
    // continue search beyond node at index i, not including i
    return binarySearchPosition(c, edgeArray, i+1, R);
  }
}

// Finds which index the node with character c should be put in edge array
// in order to maintain sorted nature
// AKA Find the smallest node that is greater than c
int findEdgeArrayPosition(node_t *addedNode, node_t *prevNode) {
  // edge array is empty
  if (prevNode->edgeCount ==0) {
    // add node to first index
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
  // Identify which index node should be placed in edge array
  int index = findEdgeArrayPosition(addedNode, prevNode);

  // resize array if edge array has reached full capacity
  resizeEdgeArray(prevNode);

  // shift array one position down, from where the new node will be put
  int i;
  for (i=(prevNode->edgeCount)-1; i>=index; i--) {
    *((prevNode->edgeArray)+i+1)=*((prevNode->edgeArray)+i);
  }

  // place node in position
  *((prevNode->edgeArray)+index) = addedNode;
  prevNode -> edgeCount ++;

}

// Preorder traversal through trie and printing each traversed node's character
void traversePrint(node_t *node) {
  // print current nodes character
  printf("%c\n", node->c);
  int i;
  // print node's children in sorted order
  for (i=0; i<node->edgeCount; i++) {
    traversePrint(*((node->edgeArray)+i));
  }
}

// Append the end symbol to the end of the word being added to trie
char *appendEndSymbol(char *word) {
  int len = strlen(word);
  // resize the word to add one more character
  word = (char*)realloc(word, (len+2)*sizeof(char));
  // add null character to signify end of word
  *(word+len+1) = NULL_CHAR;
  // append the end symbol
  *(word+len) = END_SYMBOL;
  return word;
}

// Create a trie and return it's head node
node_t *createTrie(int N) {
  // create head node
  node_t *head = createHeadNode(), *currNode, *nextNode;
  int i, j, x, charIndex;
  char* word;
  // iterate through all the words being added to trie
  for (i=0; i<N; i++) {
    word = getWord();
    word = appendEndSymbol(word);
    currNode = head;

    // update frequency of total number words
    head->freq++;

    // iterate through each character in word
    for (j=0; j<strlen(word); j++) {
      // check if node has a child node with the same character as the word
      charIndex = searchEdgeArray(currNode, *(word+j));
      // if node is found
      if (charIndex != -1) {
        // go to the matching character node
        nextNode = *((currNode->edgeArray)+charIndex);
        nextNode->freq++;
        currNode = nextNode;
        // look for next character in word
        continue;
      }
      // if node is not found
      else {
        // for remaining letters in the word
        // create nodes for them and add it to trie
        for (x=j; x<strlen(word); x++) {
          nextNode = createNode(*(word+x), currNode);
          addEdge(nextNode, currNode);
          currNode = nextNode;
        }
        break;
      }
    }
    free(word);
  }
  return head;
}

// Builds a character level trie for a given set of words
//
// The input is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Each node indicates a single character.
// Branches are ordered in alphabetic order.
//
// Outputs the pre-order traversal of the characters in the trie.
void problem_2_a() {
  int N;
  // read in the paramters
  scanf("%d\n", &N);

  // create the trie
  node_t *trieHead = createTrie(N);

  // traverse and print the nodes in the trie
  traversePrint(trieHead);

  // frees the trie
  freeTrie(trieHead);
}

// Gets the string represented by the head of the trie to the specified node
char *getFullString(node_t *node) {
  node_t *currNode = node;
  int stringLength = node->depth, i;
  char* string = malloc((stringLength+1)*sizeof(char));

  // traverse from given node to the head node
  for (i=stringLength-1; i>=0; i--) {
    // copy the node's character to the string (from end to start)
    *(string+i) = currNode->c;
    currNode = currNode->prevNode;
  }
  // add null character at the end of string
  *(string+stringLength) = '\0';

  return string;
}

// Preorder traversal through trie and print prefixes (and their frequencies)
// that have length K
void traverseK(node_t *node, int K) {
  char *string;
  int i;
  // if length of path is equal to K and node is not an end symbol
  if (K==node->depth && node->c != END_SYMBOL) {
    // get and print the prefix
    string = getFullString(node);
    printf("%s %d\n", string, node->freq);
    free(string);
  }
  // if node depth has not reached K, keeping traversing down trie
  else if (node->edgeCount != 0) {
    for (i=0; i<node->edgeCount; i++) {
      traverseK(*((node->edgeArray)+i), K);
    }
  }
}

// Outputs all prefixes of length K, in alphabetic order
// along with their frequencies
// The input will be:
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
  // scan parameters
  int N, K;
  scanf("%d %d\n", &N, &K);

  // create the trie
  node_t *trieHead = createTrie(N);

  // traverse the trie and print out the prefiexes that have length K
  traverseK(trieHead, K);

  // free trie
  freeTrie(trieHead);
}

// Find the ending node that creates the prefix 'stub'
node_t *findStubNode(node_t *node, char *stub) {
  node_t *currNode = node;
  int i, index;
  char c;
  // iterate through the characters in stub
  for (i=0; i<strlen(stub); i++) {
    c = *(stub+i);
    // character in stub matches character in node's children nodes
    if ((index=searchEdgeArray(currNode, c))!=-1) {
      // traverse to matching node
      currNode = *((currNode->edgeArray)+index);
    }
    // stub prefix does not exist in trie
    else {
      return NULL;
    }
  }
  return currNode;
}

// Collect all nodes that denote the end of a string, starting from
// a specified internal node.
// Add these nodes to an array
void traverseStub(node_t *node, node_t **array, int *track) {
  // reached the end of a string
  if (node-> c == END_SYMBOL) {
    // add node that represents the end of the string to array
    *(array+*track) = node;
    // increase tracker of array total by 1
    *track = *track + 1;
    return;
  }
  // have not reached end of string yet
  else {
    int i;
    // traverse to all of the node's children
    for (i=0; i<node->edgeCount; i++) {
      traverseStub(*((node->edgeArray)+i), array, track);
    }
  }
}

// Print the top few probable words that contain the stub prefix
// in order of descending probability and alphabetically
void printProbableWords(node_t** containStubArray, int track, int denom) {
  int i, j, maxIndex;
  float probability;
  node_t *maxNode, *currNode;
  char *string;
  // iterate through, at most, the maximum number of strings we wish to display
  for (i=0; i<P2C_COMPLETIONS && i<=track; i++) {
    // find node with maximum freq for the upcoming iteration
    maxNode = NULL;
    maxIndex = -1;
    for (j=0; j<track; j++) {
      currNode = *(containStubArray+j);
      // skip if element has already been selected
      if (currNode==NULL) {
        continue;
      }
      // check if current node is greater than max
      if (maxNode == NULL || currNode->freq>maxNode->freq) {
        // if it is, store as newest maximum node
        maxNode = currNode;
        // track its index in the array
        maxIndex = j;
      }
    }
    // calcualte the maximum freq node's probability
    probability = (float)(maxNode->freq) / (float)denom;
    // get string that node represents
    string = getFullString(maxNode->prevNode);
    printf("%.2f %s\n", probability, string);
    free(string);
    // indicate that we have already selected this node
    *(containStubArray+maxIndex) = NULL;
  }

}


// Provides a list (up to 5) of the most probable word completions for a given
// word stub.
//
// Rrobabilities are formatted to exactly 2 decimal places and are
// computed according to the following formula:
// for a word W with the prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to the program is in the form:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
void problem_2_c() {
  int N, denom;
  char *stub;
  // get parameter N
  scanf("%d\n", &N);
  // get the stub string
  stub = getWord();
  // create the trie with the inputted strings
  node_t *trieHead = createTrie(N);
  // find the node that denotes the end of the stub prefix
  node_t *stubNode = findStubNode(trieHead, stub);

  // prefix does not exist in trie, failed
  if (stubNode == NULL) {
    return;
  }

  denom = stubNode -> freq;
  // track the number of strings that contain the stub prefix
  int track = 0;
  // array contains the ending nodes of strings that contain the stub prefix
  node_t **containStub = (node_t**)malloc(sizeof(node_t*)*denom);
  // add these nodes to the array
  traverseStub(stubNode, containStub, &track);

  // print the top 5 strings
  printProbableWords(containStub, track, denom);

  free(stub);
  free(containStub);
  freeTrie(trieHead);
}
