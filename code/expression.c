#include "expression.h"

int str_len(char*str){
	int c = 0;
	while (str[c] != '\0')
		c++;
	return c;
}
void makelow(char* str){
	for(int i = 0; i<str_len(str); i++){
		str[i] = tolower(str[i]);
	}
}
int num_of_str(char*str,char ch){
	int count = 2; //beginning and endings
	for(int i=0;str[i];i++){
		if(str[i] == ch)  count++;
	}
	return count;
}
long hash(char *str){
    unsigned long hash = 5381;
    int c;
	while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
operation defineOperation(char* str) {
	if (hash(str) == hash("+") ) return ADD;
	else if (hash(str) == hash("-") ) return SUB;
	else if (hash(str) == hash("*") ) return MULT;
	else if (hash(str) == hash("/") ) return DIV;
	else if (hash(str) == hash("min") ) return MIN;
	else if (hash(str) == hash("max") ) return MAX;
	else if (hash(str) == hash("sqrt") ) return SQRT;
	else if (hash(str) == hash("exp") ) return EXP;
	else if (hash(str) == hash("ln") ) return LN;
	else if (hash(str) == hash("abs") ) return ABS;
	else if (hash(str) == hash("floor") ) return FLOOR;
	else if (hash(str) == hash("ceil") ) return CEIL;
	else return NOTUP;
}
bool  defineOperand(char* expr, double* operand){
	int dot = 0,position = (expr[0]=='-'?1:0);
	bool wr = false;
	while(position<str_len(expr)){
		if(expr[position]=='.'){
			if(position == 0 || dot == 1){
				wr = true;
				break;
			}
			else dot = 1;
		}
		else if(expr[position]<'0' && expr[position]>'9'){
			wr  =true;
			break;
		}
		position++;
	}
	if(wr==false) *operand = atof(expr);
	return wr;
}
ExpTree* newTree(){
	ExpTree* new = (ExpTree*)malloc(sizeof(ExpTree));
	new->left = NULL;
	new->right = NULL;
	new->oper = NOTUP;
	return new;
}
ExpTree* insert(char** str,int* pos){
	ExpTree* tree=newTree();
	bool binary=false;
	double a;
	int i=*pos;
	*pos+=1;
	if(str[i]==NULL) return NULL;
	operation oper=defineOperation(str[i]);
	if(oper>=ADD && oper<=MAX){
		tree->oper=oper;
		binary=true;
	}
	else if(oper>MAX && oper<NOTUP) tree->oper=oper;
	else{
		if(defineOperand(str[i],&a)==false){
			tree->oper=oper;
			tree->value=a;
		}
		else return NULL;
	}
	if(tree->oper!=NOTUP){
		if(binary==false){
				if(str[*pos]==NULL) return tree;
				tree->right=NULL;
				tree->left=insert(str,pos);
		}
		else{
			if(str[*pos]==NULL) return NULL;
			tree->right=insert(str,pos);
			if(str[*pos]==NULL) return NULL;
			tree->left=insert(str,pos);
		}
	}
	return tree;
}
char* str_cpy(char* destination, const char* source){
    if (destination == NULL) return NULL;
 	char *ptr = destination;
    while (*source != '\0'){
        *destination = *source;
        destination++;
        source++;
    }
 	*destination = '\0';
 	return ptr;
}
char** splitter(char* str){
	char** res = (char**)malloc(num_of_str(str,' ')* sizeof(char*));
	char* token=strtok(str," ");
	for(int i =0;token!=NULL;i++){
		res[i]=(char*)malloc(sizeof(char)*(str_len(token)+1));
		str_cpy(res[i],token);
		token=strtok(NULL," ");
	}
	return res;

}
ExpTree* expressionTree(char* str){
	char** splitted=splitter(str);
	int i =0;
	ExpTree* res=insert(splitted,&i);
	if(res==NULL) return NULL;
	return res;
}
double calculate(ExpTree* e){
	bool purity = true;
	if(e==NULL) return ERRMSG;
	double a,b;
	if(e->oper == ADD) a=calculate(e->right)+calculate(e->left);
	else if(e->oper == SUB) a=calculate(e->right)-calculate(e->left);
	else if(e->oper == MULT) a=calculate(e->right)*calculate(e->left);
	else if(e->oper == DIV){
		b=calculate(e->left);
		if(b==0.0) purity = false;
		a=calculate(e->right)/b;
	}
	else if(e->oper == MIN) a=fmin(calculate(e->left),calculate(e->right));
	else if(e->oper == MAX) a=fmax(calculate(e->left),calculate(e->right));
	else if(e->oper == SQRT){
		b=calculate(e->left);
		if(b<0.0) purity = false; 
		a=sqrt(b);
	}
	else if(e->oper == EXP) a=exp(calculate(e->left));
	else if(e->oper == LN){
		b=calculate(e->left);
		if(b<=0.0) purity = false; 
		a=log(b);
	}
	else if(e->oper == ABS) a=abs(calculate(e->left));
	else if(e->oper == FLOOR) a=(double)floor(calculate(e->left));
	else if(e->oper == CEIL) a=(double)ceil(calculate(e->left));
	else a = e->value;
	if(purity == false) return ERRMSG;
	return a;
}

