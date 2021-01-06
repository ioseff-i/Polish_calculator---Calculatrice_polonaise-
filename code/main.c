#include "expression.h"
int main(void) {
	ExpTree* tree = newTree();
    double result;
    char buf[BUF_SIZE];
	printf("\033[1;34m>>> \033[0m");
	while((fgets(buf,BUF_SIZE,stdin))!= NULL) {
		makelow(buf);
		tree = expressionTree(buf);
		if(tree==NULL) printf("\033[1;34m>>> \033[0m ERROR\n");
		else{
			result=calculate(tree);
			if(result==ERRMSG) printf("\033[1;34m>>> \033[0m ERROR\n");
			else printf("\033[1;34m>>> \033[0m %.4f\n",result);
		}
		free(tree);
		printf("\033[1;34m>>> \033[0m");
	}
	return 0;
}
