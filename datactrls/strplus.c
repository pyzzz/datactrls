typedef struct index_data{
	int index;
	struct index_data *next;
}index_data;

typedef struct index_list{
	struct index_data *start;
	struct index_data *end;
	int length;
}index_list;

index_data *new_index_data_p(index){
	index_data *data = malloc(sizeof(index_data));
	data->index = index;
	data->next = NULL;
	return data;
}

index_list *new_index_list_p(){
	index_list *l = malloc(sizeof(index_list));
	l->start = NULL;
	l->end = NULL;
	l->length = 0;
	return l;
}

void index_list_reset(index_list *l){
	index_data *data = l->start;
	index_data *next = NULL;
	while (data != NULL){
		next = data->next;
		free(data);
		data = next;
	}
	l->start = NULL;
	l->end = NULL;
	l->length = 0;
}

void index_list_append(index_list *l, int index){
	index_data *data = new_index_data_p(index);
	if (l->start == NULL){
		l->start = data;
	}
	if (l->end == NULL){
		l->end = data;
	}
	else{
		l->end->next = data;
		l->end = data;
	}
	l->length += 1;
}

void print_index_list(index_list *l){
	index_data *data = l->start;
	printf("index_list[%d] [", l->length);
	while (data != NULL){
		printf("%d, ", data->index);
		data = data->next;
	}
	printf("]\n");
}

index_list *new_index_list_p_from_char_find(char* value, int value_length,
	char* key, int key_length){
	index_list *l = new_index_list_p();
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] new_listmap_from_char_find: value == NULL\n");
		return l;
	}
	if (key == NULL){
		fprintf(stderr, "[error] new_listmap_from_char_find: key == NULL\n");
		return l;
	}
	#endif
	register int i;
	register int j;
	int max_i = value_length-key_length;
	char qs_table[256];
	for (i=0; i<256; i++){
		qs_table[i] = key_length+1;
	}
	for (i=0; i<key_length; i++){
		//printf("set %c %d\n", key[i], key_length-i);
		qs_table[(int)key[i]] = key_length-i;
	}
	i = 0;
	while (i <= max_i){
		//printf("qs %d %c next %c\n", i, value[i], value[i+key_length]);
		for (j=0; j<key_length; j++){
			if (value[i+j] != key[j]){
				break;
			}
		}
		if (j == key_length){
			index_list_append(l, i);
			i += key_length;
			continue;
		}
		i += qs_table[(int)value[i+key_length]];
	}
	return l;
}

int str_replace_plus(str *string, str *before, str *after){
	#if NULL_ARG_CHECK
	if (before == NULL){
		fprintf(stderr, "[error] str_replace_plus: before == NULL\n");
		return 0;
	}
	if (after == NULL){
		fprintf(stderr, "[error] str_replace_plus: after == NULL\n");
		return 0;
	}
	if (before->value == NULL){
		fprintf(stderr, "[error] str_replace_plus: before->value == NULL\n");
		return 0;
	}
	if (after->value == NULL){
		fprintf(stderr, "[error] str_replace_plus: after->value == NULL\n");
		return 0;
	}
	#endif
	index_list *l = new_index_list_p_from_char_find(
		string->value, string->length,
		before->value, before->length);
	//print_index_list(l);
	int count = l->length;
	if (count > 0){
		register int i = 0;
		register int j = 0;
		register int diff = 0;
		int new_length = string->length+(after->length-before->length)*count;
		char *new_value = malloc(sizeof(char)*(new_length+1));
		index_data *data = l->start;
		while (data != NULL){
			diff = data->index-j;
			/*fprintf(stderr, "index %d i %d j %d diff %d\n",
				data->index, i, j, diff);*/
			memcpy(new_value+i, string->value+j, diff);
			i += diff;
			j += diff;
			memcpy(new_value+i, after->value, after->length);
			i += after->length;
			j += before->length;
			data = data->next;
		}
		memcpy(new_value+i, string->value+j, string->length-j);
		str_reset_with(string, new_length, new_value);
	}
	index_list_reset(l);
	free(l);
	return count;
}

int str_replace_char_plus(str *string,
	char *before_value, char *after_value){
	str before = new_str(before_value);
	str after = new_str(after_value);
	int result_replace = str_replace_plus(string, &before, &after);
	str_reset(&before);
	str_reset(&after);
	return result_replace;
}

str new_str_from_replace_plus(str *src, str *before, str *after){
	str string = new_str_from_copy(src);
	str_replace_plus(&string, before, after);
	return string;
}

str *new_str_p_from_replace_plus(str *src, str *before, str *after){
	str *string = new_str_p_from_copy(src);
	str_replace_plus(string, before, after);
	return string;
}

str new_str_from_replace_char_plus(str *src,
	char *before_value, char *after_value){
	str string = new_str_from_copy(src);
	str_replace_char_plus(&string, before_value, after_value);
	return string;
}

str *new_str_p_from_replace_char_plus(str *src,
	char *before_value, char *after_value){
	str *string = new_str_p_from_copy(src);
	str_replace_char_plus(string, before_value, after_value);
	return string;
}
