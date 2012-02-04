struct char_replace_result{
	int index;
	int new_length;
	char *new_value;
	byte replaced;
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
	int loop_value_index = from;
	int loop_key_index = 0;
	int return_index = -1;
	if (from >= value_length){
		return return_index;
	}
	while (1){
		//printf("%d %d %d %d %d\n", value[loop_value_index], key[loop_key_index],
		//	loop_value_index, loop_key_index, return_index);
		if (value[loop_value_index] == key[loop_key_index]){
			if (loop_key_index == 0){
				return_index = loop_value_index;
			}
			loop_key_index += 1;
		}
		else if (value[loop_value_index] == key[0]){
			return_index = loop_value_index;
			loop_key_index = 1;
		}
		else{
			return_index = -1;
			loop_key_index = 0;
		}
		if (loop_key_index == key_length){ //not bug when endswith key 
			break;
		}
		else if (loop_value_index == value_length){
			return_index = -1;
			break;
		}
		loop_value_index += 1;
	}
	return return_index;
}

int char_find(char* value, int value_length, char* key, int key_length){
	return char_find_from(value, value_length, key, key_length, 0);
}

void char_clear(char *value, int value_length, char num){
	int i;
	for (i=0; i<value_length; i++){
		value[i] = num;
	}
}

/*void char_copy_range(char *src, int src_from, char *dst, int dst_from, int length){
	//fprintf(stderr, "char_copy_range: %d %s\n", length, src);
	int i;
	for (i=0; i<length; i++){
		dst[i+dst_from] = src[i+src_from];
	}
}
[2012-01-31] memcpy instead of this*/

char *char_copy(char *src, int src_length){
	int i;
	char *dst;
	dst = malloc(sizeof(char)*(src_length+1));
	for (i=0; i<src_length; i++){
		dst[i] = src[i];
	}
	dst[src_length] = '\x00';
	return dst;
}

char_replace_result char_replace_from(char* value, int value_length,
	char* before, int before_length, char* after, int after_length, int from){
	int before_index;
	char_replace_result result;
	result.index = -1;
	result.replaced = 0;
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
	result.replaced = 1;
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
