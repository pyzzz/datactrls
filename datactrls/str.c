struct str{
	int length;
	char *value;
};
typedef struct str str;
str new_str(char *value);
str *new_str_p(char *value);

void str_reset_with(str *string, int length, char *value){
	if (string == NULL){
		return;
	}
	free(string->value);
	string->length = length;
	string->value = value;
}

void str_reset(str *string){
	str_reset_with(string, 0, NULL);
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
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "str_set_bin error: value == NULL\n");
		return;
	}
	#endif
	str_reset_with(string, length, malloc(sizeof(char)*(length+1)));
	memcpy(string->value, value, length);
	string->value[length] = '\x00';
}

void str_set_char(str *string, char *value){
	str_set_bin(string, value, strlen(value));
}

void str_set(str *string, str *src){
	str_set_bin(string, src->value, src->length);
}

void str_add_bin(str *string, char *value, int value_length){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "str_add_bin error: value == NULL\n");
		return;
	}
	#endif
	int new_length = string->length + value_length;
	char *new_value = malloc(sizeof(char)*(new_length+1));
	memcpy(new_value, string->value, string->length);
	memcpy(new_value+string->length, value, value_length);
	new_value[new_length] = '\x00';
	str_reset_with(string, new_length, new_value);
}

void str_add_char(str *string, char *value){
	str_add_bin(string, value, strlen(value));
}

void str_add(str *string, str *src){
	str_add_bin(string, src->value, src->length);
}

byte str_replace_from(str *string, str *before, str *after, int from){
	char_replace_result result;
	result = char_replace_from(
		string->value, string->length,
		before->value, before->length,
		after->value, after->length, from);
	if (result.replaced > 0){
		str_reset_with(string, result.new_length, result.new_value);
	}
	return result.replaced;
}

byte str_replace_char_from(str *string,
	char *before_value, char *after_value, int from){
	str before = new_str(before_value);
	str after = new_str(after_value);
	byte result_replace = str_replace_from(string, &before, &after, from);
	str_reset(&before);
	str_reset(&after);
	return result_replace;
}

byte str_replace_once(str *string, str *before, str *after){
	return str_replace_from(string, before, after, 0);
}

byte str_replace_char_once(str *string,
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
		if (result.replaced){
			str_reset_with(string, result.new_length, result.new_value);
			from = result.index+after->length;
			count += 1;
		}
		else{
			break;
		}
	}
	return count;
}

int str_replace_char(str *string,
	char *before_value, char *after_value){
	str before = new_str(before_value);
	str after = new_str(after_value);
	int result_replace = str_replace(string, &before, &after);
	/*printf("str_replace_char before: %s\n", before->value);
	printf("str_replace_char after: %s\n", after->value);
	printf("str_replace_char string: %s\n", string->value);*/
	str_reset(&before);
	str_reset(&after);
	return result_replace;
}

byte str_mid(str *string, int start, int end){
	//return error
	if (end > string->length){
		end = string->length;
	}
	if (start < 0){
		fprintf(stderr, "[error] str_mid: start [%d] < 0\n", start);
		return -1;
	}
	else if (end < 0){
		fprintf(stderr, "[error] str_mid: end [%d] < 0\n", end);
		return -2;
	}
	else if (end < start){
		fprintf(stderr, "[error] str_mid: end [%d] < start [%d]\n", end, start);
		return -3;
	}
	if (start == 0 && end == string->length){
		return -4;
	}
	int new_length = end-start;
	char *new_value = malloc(sizeof(char)*(new_length+1));
	//char_clear(new_value, new_length+1, '\x00');
	memcpy(new_value, string->value+start, new_length);
	new_value[new_length] = '\x00';
	str_reset_with(string, new_length, new_value);
	return 0;
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

byte str_equal(str *string_x, str *string_y){
	//return bool
	if (string_x->length != string_y->length){
		return 0;
	}
	return (
		(memcmp(string_x->value, string_y->value, string_x->length) == 0) ? 1 : 0);
}

byte str_equal_bin(str *string_x, char *value, int value_length){
	//return bool
	if (string_x->length != value_length){
		return 0;
	}
	return ((memcmp(string_x->value, value, string_x->length) == 0) ? 1 : 0);
}

byte str_equal_char(str *string_x, char *value){
	//return bool
	if (string_x->length != strlen(value)){
		return 0;
	}
	return ((memcmp(string_x->value, value, string_x->length) == 0) ? 1 : 0);
}

void print_bin(char *value, int value_length){
	int i;
	char j;
	char k;
	char l;
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] print_bin: value == NULL\n");
		return;
	}
	#endif
	for (i=0; i<value_length; i++){
		j = value[i];
		if (j >= 32 && j < 127){
			printf("%c", j);
		}
		else if (j == 9){
			printf("\\t");
		}
		else if (j == 10){
			printf("\\n");
		}
		else if (j == 13){
			printf("\\r");
		}
		else{
			printf("\\x");
			k = ((unsigned char) j)/16;
			l = ((unsigned char) j)%16;
			k = (k >= 10) ? ('a' + (k - 10)) : ('0' + k);
			l = (l >= 10) ? ('a' + (l - 10)) : ('0' + l);
			printf("%c%c", k, l);
		}
	}
}

void print_str(str *string){
	print_bin(string->value, string->length);
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
	str *string = malloc(sizeof(str));
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
