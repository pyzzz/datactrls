#include "datactrls/init.c"
#include <time.h>
#define MAX_RANGE 1000000
#define MAX_LIST_GET_RANGE 10000

void test_dictree(){
	/*[2012-01-31] set 0.94s get 0.65s
	[2012-02-01] set 0.85s get 0.53s clear 0.25 (NULL_ARG_CHECK 0)
	[2012-02-02] set 0.70s get 0.39s clear 0.23 (NULL_ARG_CHECK 0 -O3)
	*/
	printf("---- test_dictree -----\n");
	int i;
	clock_t start;
	str string = new_str(NULL);
	dictree dict = new_dictree();
	start = clock();
	printf("test dictree set (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&string, i);
		dictree_set_str(&dict, &string, &string);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test dictree get (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&string, i);
		str_set_dictree_get_str(&string, &dict, &string);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test dictree clear (%d)...\t", MAX_RANGE); fflush(stdout);
	dictree_destroy(&dict);
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("\n");
	str_reset(&string);
}

void test_list(){
	/*[2012-02-04] set 0.09s get 1.70*100s each 0.05 (NULL_ARG_CHECK 0 -O3)
	*/
	printf("---- test_list -----\n");
	int i;
	clock_t start;
	list l = new_list();
	start = clock();
	printf("test list set (%d)...\t", MAX_RANGE); fflush(stdout);
	list_append_range(&l, 0, MAX_RANGE);
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test list get (%d)...\t", MAX_LIST_GET_RANGE); fflush(stdout);
	for (i=0; i<MAX_LIST_GET_RANGE; i++){
		new_int_from_list_get(&l, i);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test list each (%d)...\t", MAX_RANGE); fflush(stdout);
	list_child *m = l.start;
	while (m != NULL){
		m->data->num += 1;
		m = m->next;
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("\n");
	list_reset(&l);
}

void test_listmap(){
	/*[2012-02-04] set 0.25s get 0.01s each 0.01s (NULL_ARG_CHECK 0 -O3)
	*/
	printf("---- test_listmap -----\n");
	int i;
	clock_t start;
	listmap l = new_listmap();
	start = clock();
	printf("test listmap set (%d)...\t", MAX_RANGE); fflush(stdout);
	listmap_append_range(&l, 0, MAX_RANGE);
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test listmap get (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=0; i<MAX_RANGE; i++){
		new_int_from_listmap_get(&l, i);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test listmap each (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=0; i<MAX_RANGE; i++){
		l.map[i]->num += 1;
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("\n");
	listmap_reset(&l);
}

void test_str(){
	/*[2012-02-05] find 0.79s replace 2.30s replace_plus 0.90s (NULL_ARG_CHECK 0 -O0)
	[2012-02-05] find 0.29s replace 0.40s replace_plus 0.40s (NULL_ARG_CHECK 0 -O3)
	*/
	printf("---- test_str -----\n");
	int i;
	clock_t start;
	str string = new_str("1143534=69789+35435^(100&10+5)");
	str key = new_str(NULL);
	str rep_before = new_str("35");
	str rep_after = new_str("3535");
	start = clock();
	printf("test str find (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=0; i<MAX_RANGE; i++){
		str_set_int(&key, i);
		str_find(&string, &key);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test str replace (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=MAX_RANGE/2; i<MAX_RANGE; i++){
		str_replace(&string, &rep_before, &rep_after);
		str_replace(&string, &rep_after, &rep_before);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	start = clock();
	printf("test str replace_plus (%d)...\t", MAX_RANGE); fflush(stdout);
	for (i=MAX_RANGE/2; i<MAX_RANGE; i++){
		str_replace_plus(&string, &rep_before, &rep_after);
		str_replace_plus(&string, &rep_after, &rep_before);
	}
	printf("%.2lfs\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	printf("\n");
	str_reset(&string);
	str_reset(&key);
	str_reset(&rep_before);
	str_reset(&rep_after);
}

int main(int argc, char **argv){
	test_dictree();
	test_list();
	test_listmap();
	test_str();
	return 0;
}
