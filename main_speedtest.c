#include "superlibrary/superlibrary.c"
#include <time.h>
#define MAX_RANGE 1000000

void test_dictree(){
	/*[2012-01-31] set 0.94s get 1.50s
	*/
	printf("---- test_dictree -----\n");
	int i;
	clock_t start = clock();
	str string = new_str(NULL);
	dictree dict = new_dictree();
	printf("test dictree set...\n");
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&string, i);
		dictree_set_str(&dict, &string, &string);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("test dictree get...\n");
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&string, i);
		dictree_get_dict_node(&dict, string.value, string.length);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("\n");
	str_reset(&string);
	dictree_destroy(&dict);
}

int main(int argc, char **argv){
	test_dictree();
	return 0;
}