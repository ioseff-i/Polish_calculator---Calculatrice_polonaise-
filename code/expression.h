#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#ifndef __DS__
#define __DS__
#define ERRMSG -0.010101
#define BUF_SIZE 2048
enum operations{
	ADD,SUB,MULT,DIV,MIN,MAX,SQRT,EXP,LN,ABS,FLOOR,CEIL,NOTUP
};
typedef enum operations operation;

struct ExpTree {
	double value;
	operation oper;
	struct ExpTree* right, * left;
};
typedef struct ExpTree ExpTree;

int str_len(char*str);
void makelow(char* str);
int num_of_str(char*str,char ch);
long hash(char *str);
operation defineOperation(char* str);
bool  defineOperand(char* expr, double* operand);
ExpTree* newTree();
ExpTree* insert(char** str,int* pos);
char* str_cpy(char* destination, const char* source);
char** splitter(char* str);
ExpTree* expressionTree(char* str);
double calculate(ExpTree* e);







#endif
