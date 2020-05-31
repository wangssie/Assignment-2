/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Insert Name Here>
 */

#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

typedef struct node node_t;

typedef char data_t;

struct node{
  char c;
  int freq;
  node_t **edgeArray;
  node_t *prevNode;
  int edgeCount;
  int depth;
};

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
void problem_2_a();

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
void problem_2_b();

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
void problem_2_c();

// Compares two characters, c1 and c2 and returns:
  // 0 if they are equal
  // -1 if c1<c2
  // 1 if c1>c2
int compareChar(char c1, char c2);

// Create an array of nodes that keeps track of a certain node's children
node_t **createEdgeArray();
// Create a node:
  // c <- character stored in node
  // depth <- the level the node is on in the trie
  // prevNode <- the parent of the node
node_t *createNode(char c, node_t *prevNode);

// Create the head node of the trie, character is the START_SYMBOL (^)
node_t *createHeadNode();

// Free the edge array of the node
void freeEdgeArray(node_t *node);

// Free the node and its children
void freeNode(node_t *node);

// Free the trie and all the nodes in the trie
void freeTrie(node_t *head);

// Performs binary search of character c in edge erray
int binarySearch(char c, node_t **edgeArray, int L, int R);

// Checks if character c exists as a child node for the node
// if yes -> returns index of node in egde array
// if not -> returns -1
int searchEdgeArray(node_t *node, char c);

// Binary search function that helps findEdgeArrayPosition function
int binarySearchPosition(char c, node_t **edgeArray, int L, int R);

// Finds which index the node with character c should be put in edge array
// in order to maintain sorted nature
// AKA Find the smallest node that is greater than c
int findEdgeArrayPosition(node_t *addedNode, node_t *prevNode);

// Resize the edge array if it has reached full capacity
// Increase the size by factor until it will exceed max number of edges
void resizeEdgeArray(node_t *node);

// Add a character into the current node's array of child nodes
// Ensure that sorted nature of array is maintained
void addEdge(node_t *addedNode, node_t *prevNode);

// Preorder traversal through trie and printing each traversed node's character
void traversePrint(node_t *node);

// Append the end symbol to the end of the word being added to trie
char *appendEndSymbol(char *word);

// Create a trie and return it's head node
node_t *createTrie(int N);

// Gets the string represented by the head of the trie to the specified node
char *getFullString(node_t *node);

// Preorder traversal through trie and print prefixes (and their frequencies)
// that have length K
void traverseK(node_t *node, int K);

// Find the ending node that creates the prefix 'stub'
node_t *findStubNode(node_t *node, char *stub);

// Collect all nodes that denote the end of a string, starting from
// a specified internal node.
// Add these nodes to an array
void traverseStub(node_t *node, node_t **array, int *track);

// Print the top few probable words that contain the stub prefix
// in order of descending probability and alphabetically
void printProbableWords(node_t** containStubArray, int track, int denom);

#endif
