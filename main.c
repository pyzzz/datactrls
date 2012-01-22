#include <stdio.h>
#include <stdlib.h>
#include "superlibrary/str.c"

void test_hello_world(){
	printf("---- test_hello_world -----\n");
	str string = new_str("");
	str end = new_str("end");
	set_str(&string, "hello world");
	printf("value: %s length: %d\n", string.value, string.length);
	printf("%s\n", end.value);
	printf("\n");
}

void test_copy(){
	printf("---- test_copy -----\n");
	str src = new_str("before copy");
	str dst = new_str_from_copy(&src);
	set_str(&dst, "after copy");
	printf("src: \"%s\" dst: \"%s\"\n", src.value, dst.value);
	printf("\n");
}

void test_index(){
	printf("---- test_index -----\n");
	str string = new_str("test index");
	str key = new_str("index");
	int index = str_find(&string, &key);
	printf("find \"%s\" in \"%s\": index = %d\n", key.value, string.value, index);
	printf("\n");
}

void test_replace(){
	printf("---- test_replace -----\n");
	str replace_before = new_str("abc");
	str replace_after = new_str("abcde");
	str string_before = new_str("test replace abcabcabc");
	str string_after = new_str_from_replace(
		&string_before, &replace_before, &replace_after);
	printf("before replace: %s\n", string_before.value);
	printf("after replace: %s\n", string_after.value);
	set_str(&replace_after, "1");
	str_replace(&string_before, &replace_before, &replace_after);
	printf("replace: %s\n", string_before.value);
	printf("\n");
}

void test_mid(){
	printf("---- test_mid -----\n");
	str string = new_str("0123456789");
	str string_new = new_str_from_mid(&string, 5, 10);
	str_mid(&string, 1, 5);
	printf("new string: %s\n", string_new.value);
	printf("mid string: %s\n", string.value);
	printf("\n");
}

int main(int argc, char **argv){
	test_hello_world();
	test_copy();
	test_index();
	test_replace();
	test_mid();
	return 0;
}