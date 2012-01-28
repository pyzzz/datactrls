struct str{
	int length;
	char *value;
};
typedef struct str str;
str *new_str_p(char *value);

void str_reset(str *string){
	if (string == NULL){
		return;
	}
	free(string->value);
	string->length = 0;
	string->value = NULL;
}

int str_find_from(str *string, str *key, int start){
	return char_find_from(string->value, string->length,
		key->value, key->length, start);
}

int str_find(str *string, str *key){
	return char_find(string->value, string->length,
		key->value, key->length);
}

void str_set_bin(str *string, char *value, int length){
	if (value == NULL){
		fprintf(stderr, "str_set_bin error: value == NULL\n");
		return;
	}
	int i;
	str_reset(string);
	string->length = length;
	string->value = (char*) malloc(sizeof(char)*(length+1));
	for (i=0; i<length; i++){
		string->value[i] = value[i];
	}
	string->value[length] = '\x00';
}

void str_set_char(str *string, char *value){
	int length = char_len(value);
	str_set_bin(string, value, length);
}

void str_set(str *string, str *src){
	str_set_bin(string, src->value, src->length);
}

void str_add_bin(str *string, char *value, int value_length){
	int i;
	int new_length = string->length + value_length;
	char *new_value = (char*) malloc(sizeof(char)*(new_length+1));
	int new_value_i = 0;
	for (i=0; i<string->length; i++){
		new_value[new_value_i] = string->value[i];
		new_value_i += 1;
	}
	for (i=0; i<value_length; i++){
		new_value[new_value_i] = value[i];
		new_value_i += 1;
	}
	new_value[new_length] = '\x00';
	str_reset(string);
	string->length = new_length;
	string->value = new_value;
}

void str_add_char(str *string, char *value){
	int length = char_len(value);
	str_add_bin(string, value, length);
}

void str_add(str *string, str *src){
	str_add_bin(string, src->value, src->length);
}

int str_replace_from(str *string, str *before, str *after, int from){
	char_replace_result result;
	result = char_replace_from(
		string->value, string->length,
		before->value, before->length,
		after->value, after->length, from);
	if (result.count > 0){
		str_reset(string);
		string->length = result.new_length;
		string->value = result.new_value;
	}
	return result.count;
}

int str_replace_char_from(str *string,
	char *before_value, char *after_value, int from){
	str *before = new_str_p(before_value);
	str *after = new_str_p(after_value);
	int result_replace;
	result_replace = str_replace_from(string, before, after, from);
	str_reset(before);
	str_reset(after);
	free(before);
	free(after);
	before = NULL;
	after = NULL;
	return result_replace;
}

int str_replace_once(str *string, str *before, str *after){
	return str_replace_from(string, before, after, 0);
}

int str_replace_char_once(str *string,
	char *before_value, char *after_value){
	return str_replace_char_from(string, before_value, after_value, 0);
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
			str_reset(string);
			string->length = result.new_length;
			string->value = result.new_value;
			//printf("new %p from %p\n", string->value, result.new_value);
			from = result.index+after->length;
			count += result.count;
		}
		else{
			break;
		}
	}
	return count;
}

int str_replace_char(str *string,
	char *before_value, char *after_value){
	str *before = new_str_p(before_value);
	str *after = new_str_p(after_value);
	int result_replace;
	result_replace = str_replace(string, before, after);
	/*printf("str_replace_char before: %s\n", before->value);
	printf("str_replace_char after: %s\n", after->value);
	printf("str_replace_char string: %s\n", string->value);*/
	str_reset(before);
	str_reset(after);
	free(before);
	free(after);
	before = NULL;
	after = NULL;
	return result_replace;
}

int str_mid(str *string, int start, int end){
	if (end > string->length){
		end = string->length;
	}
	if (start < 0){
		fprintf(stderr, "[error] str_mid: start [%d] < 0\n", start);
		return 0;
	}
	else if (end < 0){
		fprintf(stderr, "[error] str_mid: end [%d] < 0\n", end);
		return 0;
	}
	else if (end < start){
		fprintf(stderr, "[error] str_mid: end [%d] < start [%d]\n", end, start);
		return 0;
	}
	int new_length = end-start;
	char *new_value = (char*) malloc(sizeof(char)*(new_length+1));
	//char_clear(new_value, new_length+1, '\x00');
	char_copy_range(string->value, start, new_value, 0, new_length);
	new_value[new_length] = '\x00';
	str_reset(string);
	string->length = new_length;
	string->value = new_value;
	return 1;
}

void str_strip(str *string){
	int new_start = 0;
	int new_end = string->length;
	int i;
	char j;
	for (i=0; i<string->length; i++){
		j = string->value[i];
		if (j != ' ' && j != '\t'){
			break;
		}
		new_start += 1;
	}
	for (i=string->length-1; i>=0; i--){
		j = string->value[i];
		if (j != ' ' && j != '\t'){
			break;
		}
		new_end -= 1;
	}
	if (new_start > new_end){
		new_start = new_end;
	}
	str_mid(string, new_start, new_end);
}

int str_equal(str *string_x, str *string_y){
	//return bool
	int i;
	if (string_x->length != string_y->length){
		return 0;
	}
	for (i=0; i<string_x->length; i++){
		if (string_x->value[i] != string_y->value[i]){
			return 0;
		}
	}
	return 1;
}

int str_equal_char(str *string_x, char *value){
	int i;
	int length = char_len(value);
	if (string_x->length != length){
		return 0;
	}
	for (i=0; i<string_x->length; i++){
		if (string_x->value[i] != value[i]){
			return 0;
		}
	}
	return 1;
}

void print_str(str *string){
	int i;
	for (i=0; i<string->length; i++){
		printf("%c", string->value[i]);
	}
}

void print_str_verbose(str *string){
	printf("\"");
	print_str(string);
	printf("\"");
	printf("[%d]", string->length);
}

void print_str_preview(str *string){
	print_str_verbose(string);
	printf("\n");
}

str new_str(char *value){
	str string;
	string.length = 0;
	string.value = NULL;
	if (value != NULL){
		str_set_char(&string, value);
	}
	return string;
}

str *new_str_p(char *value){
	str *string = (str*) malloc(sizeof(str));
	string->length = 0;
	string->value = NULL;
	if (value != NULL){
		str_set_char(string, value);
	}
	return string;
}

str new_str_from_bin(char *value, int length){
	str string = new_str(NULL);
	str_set_bin(&string, value, length);
	return string;
}

str *new_str_p_from_bin(char *value, int length){
	str *string = new_str_p(NULL);
	str_set_bin(string, value, length);
	return string;
}

str new_str_from_copy(str *src){
	str string = new_str(NULL);
	str_set(&string, src);
	return string;
}

str *new_str_p_from_copy(str *src){
	str *string = new_str_p(NULL);
	str_set_bin(string, src->value, src->length);
	return string;
}

str new_str_from_replace_from(str *src, str *before, str *after, int from){
	str string = new_str_from_copy(src);
	str_replace_from(&string, before, after, from);
	return string;
}

str *new_str_p_from_replace_from(str *src, str *before, str *after, int from){
	str *string = new_str_p_from_copy(src);
	str_replace_from(string, before, after, from);
	return string;
}

str new_str_from_replace_char_from(str *src,
	char *before_value, char *after_value, int from){
	str string = new_str_from_copy(src);
	str_replace_char_from(&string, before_value, after_value, from);
	return string;
}

str *new_str_p_from_replace_char_from(str *src,
	char *before_value, char *after_value, int from){
	str *string = new_str_p_from_copy(src);
	str_replace_char_from(string, before_value, after_value, from);
	return string;
}

str new_str_from_replace_once(str *src, str *before, str *after){
	str string = new_str_from_copy(src);
	str_replace_once(&string, before, after);
	return string;
}

str *new_str_p_from_replace_once(str *src, str *before, str *after){
	str *string = new_str_p_from_copy(src);
	str_replace_once(string, before, after);
	return string;
}

str new_str_from_replace_char_once(str *src,
	char *before_value, char *after_value){
	str string = new_str_from_copy(src);
	str_replace_char_once(&string, before_value, after_value);
	return string;
}

str *new_str_p_from_replace_char_once(str *src,
	char *before_value, char *after_value){
	str *string = new_str_p_from_copy(src);
	str_replace_char_once(string, before_value, after_value);
	return string;
}

str new_str_from_replace(str *src, str *before, str *after){
	str string = new_str_from_copy(src);
	str_replace(&string, before, after);
	return string;
}

str *new_str_p_from_replace(str *src, str *before, str *after){
	str *string = new_str_p_from_copy(src);
	str_replace(string, before, after);
	return string;
}

str new_str_from_replace_char(str *src,
	char *before_value, char *after_value){
	str string = new_str_from_copy(src);
	str_replace_char(&string, before_value, after_value);
	return string;
}

str *new_str_p_from_replace_char(str *src,
	char *before_value, char *after_value){
	str *string = new_str_p_from_copy(src);
	str_replace_char(string, before_value, after_value);
	return string;
}

str new_str_from_mid(str *src, int start, int end){
	str string = new_str_from_copy(src);
	str_mid(&string, start, end);
	return string;
}

str *new_str_p_from_mid(str *src, int start, int end){
	str *string = new_str_p_from_copy(src);
	str_mid(string, start, end);
	return string;
}
