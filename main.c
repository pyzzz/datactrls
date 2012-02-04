#include "datactrls/init.c"
/*gcc -Wall -Wformat-security -O3 -g -lm -c "%f"
gcc -Wall -Wformat-security -O3 -g -lm -o "%e" "%f"
-g debug
valgrind -v --error-limit=no --leak-check=full ./main
[2012-02-04] total heap usage: 330 allocs, 330 frees, 7,250 bytes allocated*/

void test_hello_world(){
	printf("---- test_hello_world -----\n");
	str string = new_str(NULL);
	str end = new_str("end");
	str_set_char(&string, "hello world");
	printf("value: %s length: %d\n", string.value, string.length);
	printf("%s\n", end.value);
	printf("\n");
	str_reset(&string);
	str_reset(&end);
}

void test_copy(){
	printf("---- test_copy -----\n");
	str src = new_str("before copy");
	str dst = new_str_from_copy(&src);
	str_set_char(&dst, "after copy");
	printf("src: \"%s\" dst: \"%s\"\n", src.value, dst.value);
	printf("\n");
	str_reset(&src);
	str_reset(&dst);
}

void test_index(){
	printf("---- test_index -----\n");
	str string = new_str("testiindex");
	str key = new_str("index");
	int index = str_find(&string, &key);
	printf("find \"%s\" in \"%s\": index = %d\n", key.value, string.value, index);
	printf("\n");
	str_reset(&string);
	str_reset(&key);
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
	str_reset(&replace_before);
	str_reset(&replace_after);
	str_reset(&string_before);
	str_reset(&string_after);
}

void test_mid(){
	printf("---- test_mid -----\n");
	str string = new_str("0123456789");
	str string_new = new_str_from_mid(&string, 5, 10);
	str_mid(&string, 1, 5);
	printf("new string: %s\n", string_new.value);
	printf("mid string: %s\n", string.value);
	printf("\n");
	str_reset(&string);
	str_reset(&string_new);
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
	str_reset(&string_x);
	str_reset(&string_y);
	str_reset(&string_z);
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
	int result_get = new_int_from_list_get(&l, num_get);
	int result_get_r = new_int_from_list_get(&l, num_get_r);
	int result_find = list_find_int(&l, num_find);
	printf("list get %d: %d\n", num_get, result_get);
	printf("list get %d: %d\n", num_get_r, result_get_r);
	printf("list find %d: %d\n", num_find, result_find);
	printf("\n");
	str_reset(&string);
	list_reset(&l);
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
	str result_get = new_str_from_list_get(&l, num_get);
	int result_find = list_find_char(&l, "100");
	printf("list get %d: %s\n", num_get, result_get.value);
	printf("list find %s: %d\n", "100", result_find);
	printf("\n");
	str_reset(&start);
	str_reset(&temp);
	str_reset(&result_get);
	list_reset(&l);
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
	str_reset(&row);
	list_reset(&l);
	list_reset(&ls);
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
	str_reset(&string);
}

void test_list_reset(){
	printf("---- test_list_reset -----\n");
	list l = new_list_from_split_char_skip_space("abc, 10, 5, _ _, 1", ",");
	print_list(&l);
	list_reset(&l);
	print_list(&l);
	printf("\n");
	list_reset(&l);
}

void test_convert(){
	printf("---- test_convert -----\n");
	str string = new_str_from_int(1199900);
	print_str_preview(&string);
	int integer = new_int_from_char("-1090");
	printf("%d\n", integer);
	printf("\n");
	str_reset(&string);
}

void test_str_add(){
	printf("---- test_str_add -----\n");
	str string = new_str("_0");
	str_add_char(&string, "_3210");
	print_str_preview(&string);
	printf("\n");
	str_reset(&string);
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
	str_reset(&hex);
	str_reset(&string);
}

void test_file(){
	printf("---- test_file -----\n");
	str path = new_str("abc.txt");
	file io = new_file();
	file_open(&io, path.value, "wb");
	file_write_char(&io, "abc\xff\x10");
	file_close(&io);
	str string = new_str(NULL);
	file_open(&io, path.value, "rb");
	file_read_str_all(&io, &string);
	file_close(&io);
	print_str_preview(&string);
	hex_set(&string, &string);
	print_str_preview(&string);
	file_remove(path.value);
	printf("\n");
	str_reset(&path);
	str_reset(&string);
}

void test_dictree(){
	printf("---- test_dictree -----\n");
	dictree dict = new_dictree();
	dictree_set_char(&dict, "abc", "_abc");
	dictree_set_char(&dict, "abcd", "_abcd");
	dictree_set_char(&dict, "wmwm", "_wmwm");
	dictree_set_char(&dict, "_bcd", "0");
	print_dictree(&dict);
	dictree_remove_from_char(&dict, "_bcd");
	printf("dictree node count %d\n", dictree_node_count(&dict));
	dictree_cleanup(&dict);
	printf("dictree node count %d\n", dictree_node_count(&dict));
	print_dictree(&dict);
	str string = new_str_from_dictree_get_char(&dict, "abcd");
	print_str_preview(&string);
	dictree_reset(&dict);
	print_dictree(&dict);
	printf("\n");
	dictree_destroy(&dict);
	str_reset(&string);
}

void test_extend(){
	printf("---- test_extend -----\n");
	dictree dict = new_dictree();
	dictree_set_char(&dict, "abc", "_abc");
	dictree_set_char(&dict, "abcd", "_abcd");
	dictree_set_char(&dict, "wmwm", "_wmwm");
	print_dictree(&dict);
	dictree dict_extend = new_dictree();
	dictree_set_char(&dict_extend, "abc", "_abcabc");
	dictree_set_char(&dict_extend, "zzz", "1");
	dictree_extend(&dict, &dict_extend);
	print_dictree(&dict_extend);
	dictree dict_copy = new_dictree_from_copy(&dict);
	print_dictree(&dict_copy);
	printf("\n");
	dictree_destroy(&dict);
	dictree_destroy(&dict_extend);
	dictree_destroy(&dict_copy);
}

void test_format(){
	printf("---- test_format -----\n");
	str string = new_str_from_bin("0:\x00, 1:\x01, n:\n", 13);
	dictree dict = new_dictree_from_format_str_skip_space(&string, ':', ',');
	print_dictree(&dict);
	dictree_format_char_skip_space(&dict,
		"0 = zero, 10 = ten, 100 = hundred", '=', ',');
	print_dictree(&dict);
	printf("\n");
	str_reset(&string);
	dictree_destroy(&dict);
}

void test_hash(){
	printf("---- test_hash -----\n");
	printf("hash: %ld", hash_char("abc"));
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
	test_file();
	test_dictree();
	test_extend();
	test_format();
	test_hash();
	return 0;
}