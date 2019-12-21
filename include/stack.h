#pragma once

#define MAX_CAPACITY 1000

typedef struct{
	int count;
	int item[MAX_CAPACITY];
} Stack;

Stack *InitializeStack(void);

int Empty(Stack *S);

int Pop(Stack* S);

void Push(Stack* S, int X);
