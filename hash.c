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

void hash(char* string, char **hashtable, int *M, int K);

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
    char *string;
    scanf("%s", string);
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
  scanf("%d %d %d", &N, &M, &K);
  char **hashtable = createHashtable(M);
  for (i=0; i<N; i++) {
    char *string;
    scanf("%s", string);
    hash(string, hashtable, &M, K);
  }
  printHashtable(hashtable, M);
  free(hashtable);
}

// creates a size M array of character pointers AKA the hashtable array
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

// frees the hashtable and pointers inside the hashtable
void freeHashtable(char** hashtable, int length) {
  int i;
  for (i=0; i<length; i++) {
    if (*(hashtable+i) != NULL) {
    free(*(hashtable+i));
    }
  }
  free(hashtable);
}

// print out the hashtable and the contents
void printHashtable(char** hashtable, int M) {
  int i;
  for (i=0; i<M; i++) {
    printf("%d: ", i);
    // if hash bucket is not empty
    if (*(hashtable+i)!= NULL) {
      printf("%s", *(hashtable+i));
    }
    printf("\n");
  }
}

// adds the string to the hashtable
void hash(char* string, char **hashtable, int *M, int K) {
  int originalHash = getHash(string, *M);
  int hashNumber = originalHash;
  int repeat = FALSE;
  // looking for an empty hash bucket and
  // has not reached original bucket twice
  printf("word im dealing with: %s\n", string);
  printf("original hash number: %d\n", hashNumber);
  while (!(hashNumber==originalHash && repeat)) {
    // hash bucket not empty
    if (*(hashtable+hashNumber)==NULL) {
      // add to bucket and exit the function
      printf("hash @ %d succeeded\n", hashNumber);
      *(hashtable+hashNumber) = string;
      return;
    }
    // otherwise increment +k buckets
    else {

      printf("hash number %d failed. try @ ", hashNumber);
      hashNumber = (hashNumber+K)%(*M);
      printf("%d\n", hashNumber);
      repeat = TRUE;
    }
  }
  // could not find a hash bucket
  printf("Finding empty bucket unsuccessful. Rehash entire hashtable\n");
  hashtable = rehash(hashtable, M, K);
  // hash the string again into new hashtable
  hash(string, hashtable, M, K);
}


// Rehashes the hashtable to double the original size
char** rehash(char** hashtable, int *M, int K) {
  // double hashtable size
  *M = (*M)*2;
  // create a new array of double size
  char** newHashtable = (char**)malloc((*M)*sizeof(char*));
  assert(newHashtable!=NULL);
  // go through old hashtable and hash everything into new hashtable
  int i;
  for (i=0;i<(*M)/2; i++) {
    if (*(hashtable+i)!=NULL) {
      hash(*(hashtable+i), newHashtable, M, K);
    }
  }
  // free old hashtable
  free(hashtable);
  // return new hashtable
  return newHashtable;
}
