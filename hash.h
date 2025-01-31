/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Insert Name Here>
 */

#ifndef HASH_H
#define HASH_H

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a();

// Implements a solution to Problem 1 (b), which reads in from stdin:
//   N M K
//   str_1
//   str_2
//   ...
//   str_N
// Each string is inputed (in the given order) into a hash table with size
// M. The collision resolution strategy must be linear probing with step
// size K. If an element cannot be inserted then the table size should be
// doubled and all elements should be re-hashed (in index order) before
// the element is re-inserted.
//
// This function must output the state of the hash table after all insertions
// are performed, in the following format
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void problem_1_b();

/* MY OWN FUNCTIONS */

// Maps each character to a number
// lower case a-z: 0-25
// upper case A-Z: 26-51
// digits 0-9: 52-61
int getDigit(char c);

// Gets the hash number for a string
int getHash(char *string, int M);

// Adds the string to the hashtable
// Returns pointer of hashtable in case hashtable was rehashed (new pointer)
char** hash(char* string, char **hashtable, int *M, int K);

// Rehashes the hashtable to double the original size
char** rehash(char** hashtable, int *M, int K);

// Creates a size M array of character pointers AKA the hashtable array
// initialises all the pointers to NULL
char** createHashtable(int M);

// Frees the hashtable and pointers inside the hashtable
void freeHashtable(char** hashtable, int length);

// Prints out the hashtable and the contents
void printHashtable(char** hashtable, int M);

// Scans the string from stdin and returns its pointer
char *getWord(void);

#endif
