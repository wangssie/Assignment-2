/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Insert Name Here>
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hash.h"

#define FIRST_LOWER_CHAR 'a'
#define FIRST_UPPER_CHAR 'A'
#define UPPER_CASE_START 26
#define FIRST_NUMBER '0'
#define NUMBERS_START 52
#define BINARY_BASE 64
#define FALSE 0
#define TRUE 1
#define DEFAULT_SIZE 5
#define INCREASE_MEMORY_FACTOR 2
#define REHASH_FACTOR 2

char** hash(char* string, char **hashtable, int *M, int K);

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a() {
  int N, M, i;
  scanf("%d %d\n", &N, &M);
  for (i=0; i<N; i++) {
    char *string = getWord();
    printf("%d\n", getHash(string, M));
  }

}

// Maps each character to a number
// lower case a-z: 0-25
// upper case A-Z: 26-51
// digits 0-9: 52-61
int getDigit(char c) {
  // character is a letter
  if (isalpha(c)) {
    // lower case
    if (islower(c)) {
      return (int)c - (int)FIRST_LOWER_CHAR;
    }
    // upper case
    else {
      return (int)c - (int)FIRST_UPPER_CHAR + UPPER_CASE_START;
    }
  }
  // character is a digit
  else {
    return (int)c - (int)FIRST_NUMBER + NUMBERS_START;
  }
}

// Gets the hash number for a string
int getHash(char *string, int M) {
  int len = strlen(string);
  int modfinal = getDigit(*(string+0))%M;
  int mod1, mod2, i;
  // applies Horner's rule
  // (a+b)%M= (a%M + b%M)%M
  // (a*b)%M = ((a%M)*(b%M))%M
  for (i=1; i<len; i++) {
    mod1 = (modfinal*(BINARY_BASE%M))%M;
    mod2 = getDigit(*(string+i))%M;
    modfinal = (mod1+mod2)%M;
  }
  return modfinal;
}

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
void problem_1_b() {
  int N, M, K, i;
  scanf("%d %d %d\n", &N, &M, &K);
  // create hashtable
  char **hashtable = createHashtable(M);
  char *string;
  // read in the N strings
  for (i=0; i<N; i++) {
    string = getWord();
    // add string to hashtable
    hashtable = hash(string, hashtable, &M, K);
  }
  // print the hashtable
  printHashtable(hashtable, M);
  // free all pointers (hashtable + strings)
  freeHashtable(hashtable, M);
}

// Adds the string to the hashtable
// Returns pointer of hashtable in case hashtable was rehashed (new pointer)
char** hash(char* string, char **hashtable, int *M, int K) {
  int originalHash = getHash(string, *M);
  int hashNumber = originalHash;
  int repeat = FALSE;
  // looking for an empty hash bucket and
  // has not reached original bucket twice
  while (!(hashNumber==originalHash && repeat)) {
    // hash bucket not empty
    if (*(hashtable+hashNumber)==NULL) {
      // add to bucket and exit the function
      *(hashtable+hashNumber) = string;
      return hashtable;
    }
    // otherwise increment +k buckets
    else {
      hashNumber = (hashNumber+K)%(*M);
      repeat = TRUE;
    }
  }
  // could not find a hash bucket
  hashtable = rehash(hashtable, M, K);
  // hash the string again into new hashtable
  hash(string, hashtable, M, K);
  return hashtable;
}

// Rehashes the hashtable to double the original size
char** rehash(char** hashtable, int *M, int K) {
  // increase hashtable size by rehash factor (double)
  *M = (*M)*REHASH_FACTOR;
  // create a new array of double size
  char** newHashtable = createHashtable(*M);
  assert(newHashtable!=NULL);
  // go through old hashtable and hash everything into new hashtable
  int i;
  for (i=0;i<(*M)/REHASH_FACTOR; i++) {
    if (*(hashtable+i)!=NULL) {
      hash(*(hashtable+i), newHashtable, M, K);
    }
  }
  // free old hashtable
  free(hashtable);
  // return new hashtable
  return newHashtable;
}

// Creates a size M array of character pointers AKA the hashtable array
// initialises all the pointers to NULL
char** createHashtable(int M) {
  char **hashtable = (char**)malloc(M*sizeof(char*));
  assert(hashtable!=NULL);
  int i;
  // initalise all elements in array to NULL
  for (i=0; i<M; i++) {
    *(hashtable+i) = NULL;
  }
  return hashtable;
}

// Frees the hashtable and pointers inside the hashtable
void freeHashtable(char** hashtable, int length) {
  int i;
  // free all the strings in the hashtable
  for (i=0; i<length; i++) {
    if (*(hashtable+i) != NULL) {
    free(*(hashtable+i));
    }
  }
  // free the hashtable itself
  free(hashtable);
}

// Prints out the hashtable and the contents
void printHashtable(char** hashtable, int M) {
  int i;
  for (i=0; i<M; i++) {
    printf("%d: ", i);
    // if hash bucket is not empty
    if (*(hashtable+i)!= NULL) {
      // print out string in that bucket
      printf("%s", *(hashtable+i));
    }
    printf("\n");
  }
}

// Scans the string from stdin and returns its pointer
char *getWord(void) {
  int size = DEFAULT_SIZE;
  int len = 0;
  char *string;
  string = (char*)malloc(sizeof(char)*size);
  assert(string!=NULL);
  int c;

  // while the next character is not a newline character
  while ((c=getchar())!=EOF && c!='\n') {
    *(string+len) = c;
    len ++;
    // assign larger memory to pointer if array has reached full capacity
    if (len == size) {
      // double the memory size
      size*=INCREASE_MEMORY_FACTOR;
      string = realloc(string, sizeof(char)*(size));
    }
  }
  // insert null character at end of string
  *(string+len) = '\0';
  return string;
}
