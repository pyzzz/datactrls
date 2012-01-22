#include <stdlib.h>
#include "char.c"

struct str{
	int length;
	char *value;
};
typedef struct str str;

int str_find_from(str *string, str *key, int start){
	return char_find_from(string->value, string->length,
		key->value, key->length, start);
}

int str_find(str *string, str *key){
	return char_find(string->value, string->length,
		key->value, key->length);
}

void set_bin_without_free(str *string, char *value, int length){
	int i;
	string->length = length;
	string->value = (char*) malloc(sizeof(char)*(length+1));
	for (i=0; i<length; i++){
		string->value[i] = value[i];
	}
	string->value[length] = '\x00';
}

void set_str_without_free(str *string, char *value){
	int length = char_len(value);
	set_bin_without_free(string, value, length);
}

void set_str(str *string, char *value){
	free(string->value); //bug if string not from new_str ?
	set_str_without_free(string, value);
}

void set_bin(str *string, char *value, int length){
	free(string->value);
	set_bin_without_free(string, value, length);
}

int str_replace_from(str *string, str *before, str *after, int from){
	char_replace_result result;
	result = char_replace_from(
		string->value, string->length,
		before->value, before->length,
		after->value, after->length, from);
	if (result.count > 0){
		free(string->value);
		string->value = result.new_value;
		string->length = result.new_length;
	}
	return result.count;
}

int str_replace_once(str *string, str *before, str *after){
	return str_replace_from(string, before, after, 0);
}

int str_replace(str *string, str *before, str *after){
	//replace all
	char_replace_result result;
	int from = 0;
	int count = 0;
	while (1){
		result = char_replace_from(
			string->value, string->length,
			before->value, before->length,
			after->value, after->length, from);
		if (result.count > 0){
			//printf("free %p\n", string->value);
			free(string->value);
			string->value = result.new_value;
			//printf("new %p from %p\n", string->value, result.new_value);
			string->length = result.new_length;
			from = result.index+after->length;
			count += result.count;
		}
		else{
			break;
		}
	}
	return count;
}

int str_mid(str *string, int start, int end){
	if (end > string->length){
		end = string->length;
	}
	if (start < 0){
		fprintf(stderr, "[error] str_mid: start < 0 [%d]\n", start);
		return 0;
	}
	if (end < 0){
		fprintf(stderr, "[error] str_mid: end < 0 [%d]\n", end);
		return 0;
	}
	if (end < start){
		fprintf(stderr, "[error] str_mid: end < start [%d, %d]\n", start, end);
		return 0;
	}
	int new_length = end-start;
	char *new_value = (char*) malloc(sizeof(char)*(new_length+1));
	char_clear(new_value, new_length+1, '\x00');
	char_copy_range(string->value, start, new_value, 0, new_length);
	free(string->value);
	string->length = new_length;
	string->value = new_value;
	return 1;
}

str new_str(char *value){
	str string;
	set_str_without_free(&string, value);
	return string;
}

str new_str_from_copy(str *src){
	str string = new_str("");
	set_bin(&string, src->value, src->length);
	return string;
}

str new_str_from_replace_from(str *src, str *before, str *after, int from){
	str string = new_str_from_copy(src);
	str_replace_from(&string, before, after, from);
	return string;
}

str new_str_from_replace_once(str *src, str *before, str *after){
	str string = new_str_from_copy(src);
	str_replace_once(&string, before, after);
	return string;
}

str new_str_from_replace(str *src, str *before, str *after){
	str string = new_str_from_copy(src);
	str_replace(&string, before, after);
	return string;
}

str new_str_from_mid(str *src, int start, int end){
	str string = new_str_from_copy(src);
	str_mid(&string, start, end);
	return string;
}
