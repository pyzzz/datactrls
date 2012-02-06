struct char_replace_result{
	int index;
	int new_length;
	char *new_value;
	int count;
};
typedef struct char_replace_result char_replace_result;

/*int char_len(char *value){
	int length = 0;
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "char_len error: value == NULL\n");
		return length;
	}
	#endif
	while (1){
		if (value[length] != '\x00'){
			length++;
		}
		else{
			break;
		}
	}
	return length;
}
[2012-01-31] strlen instead of this*/

int char_find_from(char* value, int value_length,
	char* key, int key_length, int from){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] char_find_from: value == NULL\n");
		return -1;
	}
	if (key == NULL){
		fprintf(stderr, "[error] char_find_from: key == NULL\n");
		return -1;
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
	i = from;
	while (i <= max_i){
		//printf("qs %d %c next %c\n", i, value[i], value[i+key_length]);
		for (j=0; j<key_length; j++){
			if (value[i+j] != key[j]){
				break;
			}
		}
		if (j == key_length){
			return i;
		}
		i += qs_table[(int)value[i+key_length]];
	}
	return -1;
}

int char_find(char* value, int value_length, char* key, int key_length){
	return char_find_from(value, value_length, key, key_length, 0);
}

/*void char_copy_range(char *src, int src_from, char *dst, int dst_from, int length){
	//fprintf(stderr, "char_copy_range: %d %s\n", length, src);
	int i;
	for (i=0; i<length; i++){
		dst[i+dst_from] = src[i+src_from];
	}
}

char *char_copy(char *src, int src_length){
	int i;
	char *dst;
	#if NULL_ARG_CHECK
	if (src == NULL){
		fprintf(stderr, "[error] char_copy: src == NULL\n");
		return NULL;
	}
	#endif
	dst = malloc(sizeof(char)*(src_length+1));
	for (i=0; i<src_length; i++){
		dst[i] = src[i];
	}
	dst[src_length] = '\x00';
	return dst;
}
[2012-01-31] memcpy instead of this*/

char_replace_result char_replace_from(char* value, int value_length,
	char* before, int before_length, char* after, int after_length, int from){
	int before_index;
	char_replace_result result;
	result.index = -1;
	result.count = 0;
	result.new_value = NULL;
	result.new_length = 0;
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] char_replace_from: value == NULL\n");
		return result;
	}
	if (before == NULL){
		fprintf(stderr, "[error] char_replace_from: before == NULL\n");
		return result;
	}
	if (after == NULL){
		fprintf(stderr, "[error] char_replace_from: after == NULL\n");
		return result;
	}
	#endif
	before_index = char_find_from(value, value_length, before, before_length, from);
	if (before_index == -1){
		return result;
	}
	result.index = before_index;
	result.count = 1;
	result.new_length = value_length+after_length-before_length;
	result.new_value = malloc(sizeof(char)*(result.new_length+1));
	memcpy(result.new_value, value, before_index); //"ab"
	memcpy(result.new_value+before_index, after, after_length); //"123"
	memcpy(result.new_value+before_index+after_length,
		value+before_index+before_length,
		value_length-before_index-before_length); //"de"
	result.new_value[result.new_length] = '\x00';
	return result;
}