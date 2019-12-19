#include <stdio.h>
#include <stdlib.h>
#include "../include/stack.h"

Stack* InitializeStack(void) {
  Stack* S = malloc(sizeof(Stack));
	S->count = 0;
}

int Empty(Stack *S) {
	return (S->count == 0);
}

int Pop(Stack* S){
  int X;
  if (Empty(S))
    printf("Attempted to pop from an empty stack\n");
  else {
    (S->count)--;
  	X = S->item[S->count];
  }
  return X;
}

void Push(Stack* S, int X) {
	S->item[S->count] = X;
	(S->count)++;
}
