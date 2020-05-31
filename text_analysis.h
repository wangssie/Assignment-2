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

node_t **createEdgeArray();

node_t *createNode(char c, node_t *prevNode);

node_t *createHeadNode();

void freeEdgeArray(node_t *node);

void freeNode(node_t *node);

void freeTrie(node_t *head);

int binarySearch(char c, node_t **edgeArray, int L, int R);

int searchEdgeArray(node_t *node, char c);

int binarySearchPosition(char c, node_t **edgeArray, int L, int R);

int findEdgeArrayPosition(node_t *addedNode, node_t *prevNode);

void resizeEdgeArray(node_t *node);

void addEdge(node_t *addedNode, node_t *prevNode);

void traversePrint(node_t *node);

void addEndNode(node_t *lastNode);

node_t *createTrie(int N);

#endif
