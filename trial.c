#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define FIRST_LOWER_CHAR 'a'
#define FIRST_UPPER_CHAR 'A'
#define UPPER_CASE_START 26
#define NUMBERS_START 52
#define FIRST_NUMBER '0'

int getDigit(char c) {
  if (isalpha(c)) {
    if (islower(c)) {
      return (int)c - (int)FIRST_LOWER_CHAR;
    }
    else {
      return (int)c - (int)FIRST_UPPER_CHAR + UPPER_CASE_START;
    }
  }
  else {
    return (int)c - (int)FIRST_NUMBER + NUMBERS_START;
  }

}

void problem_1_a() {
  int N, M;
  scanf("%d %d\n", &N, &M);
  int i;
  char *string;
  for (i=0; i<N; i++) {
    scanf("%s", string);
    printf("%s: length %d\n", string, strlen(string));
    printf("2nd letter in string: %c, getDigit(%c) = %d\n", *(string+1), *(string+1), getDigit(*(string+1)));
  }

}

char** createHashArray(int M) {
  char **hasharray = (char**)malloc(M*sizeof(char*));
  assert(hasharray!=NULL);
  int i;
  for (i=0; i<M; i++) {
    *hasharray = NULL;
  }
  return hasharray;
}

int main (int argc, char *argv[]) {

  printf("successful? %s", NULL);
  char** trial = createHashArray(3);
  return 0;
}
