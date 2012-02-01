#include "superlibrary/superlibrary.c"
#include <time.h>
#define MAX_RANGE 1000000

void test_dictree(){
	/*[2012-01-31] set 0.94s get 0.65s
	[2012-02-01] set 0.85s get 0.55s clear 0.25 (NULL_ARG_CHECK 0)
	*/
	printf("---- test_dictree -----\n");
	int i;
	clock_t start;
	str string = new_str(NULL);
	dictree dict = new_dictree();
	start = clock();
	printf("test dictree set...\n");
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&string, i);
		dictree_set_str(&dict, &string, &string);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test dictree get...\n");
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&string, i);
		dictree_set_str_from_str(&string, &dict, &string);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test dictree clear...\n");
	dictree_destroy(&dict);
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("\n");
	str_reset(&string);
}

int main(int argc, char **argv){
	test_dictree();
	return 0;
}
