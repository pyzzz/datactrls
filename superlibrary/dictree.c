//reference from chatty (http://0xcc.net/chatty/)
struct dict_node{
	str *key;
	str *value;
	char symbol;
	struct dict_node *next;
	struct dict_node *child;
};
typedef struct dict_node dict_node;

struct dictree{
	struct dict_node *root;
};
typedef struct dictree dictree;

dict_node *new_dict_node_p(char symbol){
	dict_node *node = malloc(sizeof(dict_node));
	node->symbol = symbol;
	node->next = NULL;
	node->child = NULL;
	node->key = NULL;
	node->value = NULL;
	return node;
}

dictree new_dictree(){
	dictree dict;
	dict.root = new_dict_node_p('\x00');
	return dict;
}

dictree *new_dictree_p(){
	dictree *dict = malloc(sizeof(dictree));
	dict->root = new_dict_node_p('\x00');
	return dict;
}

dict_node *dict_node_get(register dict_node *node, register char symbol){
	if (symbol == node->symbol){
		return node;
	}
	else if (node->next != NULL){
		return dict_node_get(node->next, symbol);
	}
	else{
		return NULL;
	}
}

dict_node *dict_node_get_last(register dict_node *node){
	if (node->next != NULL){
		return dict_node_get_last(node->next);
	}
	else{
		return node;
	}
}

void dict_node_reset(register dict_node *node){
	if (node == NULL){
		return;
	}
	node->symbol = '\x00';
	if (node->next != NULL){
		dict_node_reset(node->next);
		free(node->next);
		node->next = NULL;
	}
	if (node->child != NULL){
		dict_node_reset(node->child);
		free(node->child);
		node->child = NULL;
	}
	if (node->key != NULL){
		str_reset(node->key);
		free(node->key);
		node->key = NULL;
	}
	if (node->value != NULL){
		str_reset(node->value);
		free(node->value);
		node->value = NULL;
	}
}

void dictree_reset(dictree *dict){
	dict_node_reset(dict->root);
}

void dictree_destroy(dictree *dict){
	dictree_reset(dict);
	free(dict->root);
	dict->root = NULL;
}

void dictree_set_bin(dictree *dict, char *key, int key_length,
	char *value, int value_length){
	#if NULL_ARG_CHECK
	if (key == NULL){
		fprintf(stderr, "[error] dictree_set_bin: key == NULL\n");
		return;
	}
	if (value == NULL){
		fprintf(stderr, "[error] dictree_set_bin: value == NULL\n");
		return;
	}
	if (dict == NULL){
		fprintf(stderr, "[error] dictree_set_bin: dict == NULL\n");
		return;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] dictree_set_bin: dict->root == NULL\n");
		return;
	}
	#endif
	int i;
	char j;
	register dict_node *node = NULL;
	register dict_node *node_next = NULL;
	register dict_node *node_last = NULL;
	node = dict->root;
	for (i=0; i<key_length; i++){
		j = key[i];
		if (node->child == NULL){
			node->child = new_dict_node_p(j);
		}
		node_next = dict_node_get(node->child, j);
		if (node_next == NULL){
			node_next = new_dict_node_p(j);
			node_last = dict_node_get_last(node->child);
			node_last->next = node_next;
		}
		//printf("%c %p %p\n", j, node->child, node_next);
		node = node_next;
	}
	if (node->key != NULL){
		str_set_bin(node->key, key, key_length);
	}
	else{
		node->key = new_str_p_from_bin(key, key_length);
	}
	if (node->value != NULL){
		str_set_bin(node->value, value, value_length);
	}
	else{
		node->value = new_str_p_from_bin(value, value_length);
	}
}

void dictree_set_char(dictree *dict, char *key, char *value){
	dictree_set_bin(dict, key, strlen(key), value, strlen(value));
}

void dictree_set_str(dictree *dict, str *key, str *value){
	dictree_set_bin(dict, key->value, key->length, value->value, value->length);
}

void dictree_set_char_str(dictree *dict, char *key, str *value){
	dictree_set_bin(dict, key, strlen(key), value->value, value->length);
}

dict_node *dictree_get_dict_node(dictree *dict, char *key, int key_value){
	#if NULL_ARG_CHECK
	if (key == NULL){
		fprintf(stderr, "[error] dictree_get_dict_node: key == NULL\n");
		return NULL;
	}
	if (dict == NULL){
		fprintf(stderr, "[error] dictree_get_dict_node: dict == NULL\n");
		return NULL;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] dictree_get_dict_node: dict->root == NULL\n");
		return NULL;
	}
	#endif
	int i;
	register dict_node *node = dict->root;
	for (i=0; i<key_value; i++){
		if (!(node != NULL && node->child != NULL)){
			return NULL;
		}
		node = dict_node_get(node->child, key[i]);
	}
	return node;
}

str dictree_get_str_from_bin(dictree *dict, char *key, int key_value){
	str string = new_str(NULL);
	dict_node *node = dictree_get_dict_node(dict, key, key_value);
	if (node != NULL && node->key != NULL && node->value != NULL
		&& str_equal_bin(node->key, key, key_value)){
		str_set_bin(&string, node->value->value, node->value->length);
	}
	return string;
}

str dictree_get_str_from_char(dictree *dict, char *key){
	return dictree_get_str_from_bin(dict, key, strlen(key));
}

str dictree_get_str_from_str(dictree *dict, str *key){
	return dictree_get_str_from_bin(dict, key->value, key->length);
}

byte dictree_remove_from_bin(dictree *dict, char *key, int key_length){
	//return error
	dict_node *node = dictree_get_dict_node(dict, key, key_length);
	if (node == NULL || (node->key == NULL && node->value == NULL)){
		fprintf(stderr, "[error] dictree_remove_from_bin: %s not in dict\n", key);
		return -1;
	}
	#if NULL_ARG_CHECK
	if (dict == NULL){
		fprintf(stderr, "[error] dictree_remove_from_bin: dict == NULL\n");
		return -2;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] dictree_remove_from_bin: dict->root == NULL\n");
		return -3;
	}
	#endif
	if (node->key != NULL){
		str_reset(node->key);
		free(node->key);
		node->key = NULL;
	}
	if (node->value != NULL){
		str_reset(node->value);
		free(node->value);
		node->value = NULL;
	}
	return 0;
}

byte dictree_remove_from_char(dictree *dict, char *key){
	return dictree_remove_from_bin(dict, key, strlen(key));
}

byte dictree_remove_from_str(dictree *dict, str *key){
	return dictree_remove_from_bin(dict, key->value, key->length);
}

void print_dict_node(dict_node *node){
	if (node->key != NULL && node->value != NULL){
		print_str_verbose(node->key);
		printf(": ");
		print_str_verbose(node->value);
		printf(", ");
	}
	if (node->next != NULL){
		print_dict_node(node->next);
	}
	if (node->child != NULL){
		print_dict_node(node->child);
	}
}

void print_dictree(dictree *dict){
	#if NULL_ARG_CHECK
	if (dict == NULL){
		fprintf(stderr, "[error] print_dictree: dict == NULL\n");
		return;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] print_dictree: dict->root == NULL\n");
		return;
	}
	#endif
	printf("{");
	print_dict_node(dict->root);
	printf("}");
	printf("\n");
}

void dict_node_extend(dict_node *node, dictree *dst){
	if (node->key != NULL && node->value != NULL){
		dictree_set_str(dst, node->key, node->value);
	}
	if (node->next != NULL){
		dict_node_extend(node->next, dst);
	}
	if (node->child != NULL){
		dict_node_extend(node->child, dst);
	}
}

void dictree_extend(dictree *src, dictree *dst){
	#if NULL_ARG_CHECK
	if (src == NULL){
		fprintf(stderr, "[error] dictree_extend: src == NULL\n");
		return;
	}
	if (dst == NULL){
		fprintf(stderr, "[error] dictree_extend: dst == NULL\n");
		return;
	}
	if (src->root == NULL){
		fprintf(stderr, "[error] dictree_extend: src->root == NULL\n");
		return;
	}
	if (dst->root == NULL){
		fprintf(stderr, "[error] dictree_extend: dst->root == NULL\n");
		return;
	}
	#endif
	dict_node_extend(src->root, dst);
}

dictree new_dictree_from_copy(dictree *src){
	dictree dict = new_dictree();
	dictree_extend(src, &dict);
	return dict;
}

dictree *new_dictree_p_from_copy(dictree *src){
	dictree *dict = new_dictree_p();
	dictree_extend(src, dict);
	return dict;
}

void dictree_format_bin(dictree *dict, char *value, int value_length,
	char key_after, char value_after, byte skip_space){
	int i;
	char j;
	byte mode = 0; // 0: key, 1: value
	int key_buf_i = 0;
	int value_buf_i = 0;
	char *key_buf = malloc(sizeof(char)*(value_length+1));
	char *value_buf = malloc(sizeof(char)*(value_length+1));
	str key_str = new_str(NULL);
	str value_str = new_str(NULL);
	for (i=0; i<value_length; i++){
		j = value[i];
		if (j == key_after){
			if (mode == 1){
				fprintf(stderr,
				"[error] dictree_set_format_from_bin: format error 0 [%s]\n",
				value);
				break;
			}
			mode = 1;
		}
		else if (j == value_after){
			if (key_buf_i == 0 || value_buf_i == 0){
				fprintf(stderr,
				"[error] dictree_set_format_from_bin: format error 1 [%s]\n",
				value);
				break;
			}
			mode = 0;
			key_buf[key_buf_i] = '\x00';
			value_buf[value_buf_i] = '\x00';
			if (skip_space != 0){
				str_set_bin(&key_str, key_buf, key_buf_i);
				str_set_bin(&value_str, value_buf, value_buf_i);
				str_strip(&key_str);
				str_strip(&value_str);
				dictree_set_str(dict, &key_str, &value_str);
			}
			else{
				dictree_set_bin(dict, key_buf, key_buf_i, value_buf, value_buf_i);
			}
			key_buf_i = 0;
			value_buf_i = 0;
			key_buf[0] = '\x00';
			value_buf[0] = '\x00';
		}
		else if (mode == 0){
			key_buf[key_buf_i] = j;
			key_buf_i += 1;
		}
		else if (mode == 1){
			value_buf[value_buf_i] = j;
			value_buf_i += 1;
		}
	}
	if (key_buf_i != 0 && value_buf_i != 0){
		if (skip_space != 0){
			str_set_bin(&key_str, key_buf, key_buf_i);
			str_set_bin(&value_str, value_buf, value_buf_i);
			str_strip(&key_str);
			str_strip(&value_str);
			dictree_set_str(dict, &key_str, &value_str);
		}
		else{
			dictree_set_bin(dict, key_buf, key_buf_i, value_buf, value_buf_i);
		}
	}
	free(key_buf);
	free(value_buf);
	str_reset(&key_str);
	str_reset(&value_str);
}

void dictree_format_char(dictree *dict, char *value,
	char key_after, char value_after){
	dictree_format_bin(dict, value, strlen(value), key_after, value_after, 0);
}

void dictree_format_str(dictree *dict, str *string,
	char key_after, char value_after){
	dictree_format_bin(dict,
		string->value, string->length, key_after, value_after, 0);
}

void dictree_format_bin_skip_space(dictree *dict, char *value, int value_length,
	char key_after, char value_after){
	dictree_format_bin(dict, value, value_length, key_after, value_after, 1);
}

void dictree_format_char_skip_space(dictree *dict, char *value,
	char key_after, char value_after){
	dictree_format_bin(dict, value, strlen(value), key_after, value_after, 1);
}

void dictree_format_str_skip_space(dictree *dict, str *string,
	char key_after, char value_after){
	dictree_format_bin(dict,
		string->value, string->length, key_after, value_after, 1);
}

dictree new_dictree_from_format_bin(char *value, int value_length,
	char key_after, char value_after){
	dictree dict = new_dictree();
	dictree_format_bin(&dict, value, value_length, key_after, value_after, 0);
	return dict;
}

dictree *new_dictree_p_from_format_bin(char *value, int value_length,
	char key_after, char value_after){
	dictree *dict = new_dictree_p();
	dictree_format_bin(dict, value, value_length, key_after, value_after, 0);
	return dict;
}

dictree new_dictree_from_format_char(char *value,
	char key_after, char value_after){
	dictree dict = new_dictree();
	dictree_format_bin(&dict, value, strlen(value), key_after, value_after, 0);
	return dict;
}

dictree *new_dictree_p_from_format_char(char *value,
	char key_after, char value_after){
	dictree *dict = new_dictree_p();
	dictree_format_bin(dict, value, strlen(value), key_after, value_after, 0);
	return dict;
}

dictree new_dictree_from_format_str(str *string,
	char key_after, char value_after){
	dictree dict = new_dictree();
	dictree_format_bin(&dict,
		string->value, string->length, key_after, value_after, 0);
	return dict;
}

dictree *new_dictree_p_from_format_str(str *string,
	char key_after, char value_after){
	dictree *dict = new_dictree_p();
	dictree_format_bin(dict,
		string->value, string->length, key_after, value_after, 0);
	return dict;
}

dictree new_dictree_from_format_bin_skip_space(char *value, int value_length,
	char key_after, char value_after){
	dictree dict = new_dictree();
	dictree_format_bin(&dict, value, value_length, key_after, value_after, 1);
	return dict;
}

dictree *new_dictree_p_from_format_bin_skip_space(char *value, int value_length,
	char key_after, char value_after){
	dictree *dict = new_dictree_p();
	dictree_format_bin(dict, value, value_length, key_after, value_after, 1);
	return dict;
}


dictree new_dictree_from_format_char_skip_space(char *value,
	char key_after, char value_after){
	dictree dict = new_dictree();
	dictree_format_bin(&dict, value, strlen(value), key_after, value_after, 1);
	return dict;
}

dictree *new_dictree_p_from_format_char_skip_space(char *value,
	char key_after, char value_after){
	dictree *dict = new_dictree_p();
	dictree_format_bin(dict, value, strlen(value), key_after, value_after, 1);
	return dict;
}

dictree new_dictree_from_format_str_skip_space(str *string,
	char key_after, char value_after){
	dictree dict = new_dictree();
	dictree_format_bin(&dict,
		string->value, string->length, key_after, value_after, 1);
	return dict;
}

dictree *new_dictree_p_from_format_str_skip_space(str *string,
	char key_after, char value_after){
	dictree *dict = new_dictree_p();
	dictree_format_bin(dict,
		string->value, string->length, key_after, value_after, 1);
	return dict;
}
