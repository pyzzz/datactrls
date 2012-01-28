#include <stdio.h>
#include <stdlib.h>
#include "superlibrary/superlibrary.c"
//gcc -Wall -lm -c "%f"
//gcc -Wall -lm -o "%e" "%f"

void test_hello_world(){
	printf("---- test_hello_world -----\n");
	str string = new_str(NULL);
	str end = new_str("end");
	str_set_char(&string, "hello world");
	printf("value: %s length: %d\n", string.value, string.length);
	printf("%s\n", end.value);
	printf("\n");
}

void test_copy(){
	printf("---- test_copy -----\n");
	str src = new_str("before copy");
	str dst = new_str_from_copy(&src);
	str_set_char(&dst, "after copy");
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
	str_replace_char(&string_before, "abc", "1");
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

void test_equal(){
	printf("---- test_equal -----\n");
	str string_x = new_str("abcde");
	str string_y = new_str("123");
	str string_z = new_str("abcde");
	int result;
	result = str_equal(&string_x, &string_y);
	printf("str_equal string_x string_y: %d\n", result);
	result = str_equal(&string_x, &string_z);
	printf("str_equal string_x string_z: %d\n", result);
	result = str_equal_char(&string_x, "abcde");
	printf("str_equal_char string_x \"abcde\": %d\n", result);
	printf("\n");
}

void test_list(){
	printf("---- test_list -----\n");
	list l = new_list_from_range(1, 11);
	str string = new_str("abc");
	list_remove(&l, 1);
	list_remove_int(&l, 10);
	list_append_str(&l, &string);
	print_list(&l);
	int num_get = 1;
	int num_get_r = 7;
	int num_find = 5;
	int result_get = list_get_int(&l, num_get);
	int result_get_r = list_get_int(&l, num_get_r);
	int result_find = list_find_int(&l, num_find);
	printf("list get %d: %d\n", num_get, result_get);
	printf("list get %d: %d\n", num_get_r, result_get_r);
	printf("list find %d: %d\n", num_find, result_find);
	printf("\n");
}

void test_str_list(){
	printf("---- test_str_list -----\n");
	list l = new_list();
	str start = new_str("start");
	str temp = new_str("temp");
	list_append_int(&l, 0);
	list_append_int(&l, 123);
	list_append_int(&l, 500);
	list_append_char(&l, "end");
	list_insert_str(&l, &start, 0);
	list_insert_str(&l, &temp, 3);
	list_insert_char(&l, "100", 3);
	list_remove_char(&l, "temp");
	list_insert_int(&l, 100, 3);
	print_list(&l);
	int num_get = 0;
	str result_get = list_get_str(&l, num_get);
	int result_find = list_find_char(&l, "100");
	printf("list get %d: %s\n", num_get, result_get.value);
	printf("list find %s: %d\n", "100", result_find);
	printf("\n");
}

void test_split(){
	printf("---- test_split -----\n");
	str row = new_str_from_bin("_123___ ___ ___ 0 	0___ \x00_1", 27);
	list l = new_list_from_split_str_skip_space(&row, "___");
	print_list(&l);
	list_remove_all_char(&l, "");
	print_list(&l);
	list ls = new_list_from_split_bin_space("id name 	cost\x00.date", 20);
	print_list(&ls);
	printf("\n");
}

void test_strip(){
	printf("---- test_strip -----\n");
	str string = new_str(" strip test 	");
	str_strip(&string);
	print_str_preview(&string);
	str_set_char(&string, "  	");
	str_strip(&string);
	print_str_preview(&string);
	printf("\n");
}

void test_list_reset(){
	printf("---- test_list_reset -----\n");
	list l = new_list_from_split_char_skip_space("abc, 10, 5, _ _, 1", ",");
	print_list(&l);
	list_reset(&l);
	print_list(&l);
	printf("\n");
}

void test_convert(){
	printf("---- test_convert -----\n");
	str string = new_str_from_int(1199900);
	print_str_preview(&string);
	int integer = new_int_from_char("-1090");
	printf("%d\n", integer);
	printf("\n");
}

void test_str_add(){
	printf("---- test_str_add -----\n");
	str string = new_str("_0");
	str_add_char(&string, "_3210");
	print_str_preview(&string);
	printf("\n");
}

void test_hex(){
	printf("---- test_hex -----\n");
	str hex = new_hex_from_char("ul101wm");
	print_str_preview(&hex);
	str string = new_str_from_hex(&hex);
	print_str_preview(&string);
	str_set_hex_char(&string, "5051525300");
	print_str_preview(&string);
	printf("\n");
}

int main(int argc, char **argv){
	test_hello_world();
	test_copy();
	test_index();
	test_replace();
	test_mid();
	test_equal();
	test_list();
	test_str_list();
	test_split();
	test_strip();
	test_list_reset();
	test_convert();
	test_str_add();
	test_hex();
	return 0;
}
