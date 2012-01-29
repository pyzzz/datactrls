//reference from chatty (http://0xcc.net/chatty/)
struct dict_node{
	char symbol;
	str *key;
	str *value;
	struct dict_node *next;
	struct dict_node *child;
};
typedef struct dict_node dict_node;

struct dictree{
	struct dict_node *root;
};
typedef struct dictree dictree;

dict_node *new_dict_node_p(char symbol){
	dict_node *node = (dict_node*) malloc(sizeof(dict_node));
	node->symbol = symbol;
	node->next = NULL;
	node->child = NULL;
	node->key = NULL;
	node->value = NULL;
	return node;
}

dictree new_dict(){
	dictree dict;
	dict.root = new_dict_node_p('\x00');
	return dict;
}

dictree *new_dict_p(){
	dictree *dict = (dictree*) malloc(sizeof(dictree));
	dict->root = new_dict_node_p('\x00');
	return dict;
}

dict_node *dict_node_get(dict_node *node, char symbol){
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

dict_node *dict_node_get_last(dict_node *node){
	if (node->next != NULL){
		return dict_node_get_last(node->next);
	}
	else{
		return node;
	}
}

void dict_node_reset(dict_node *node){
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

void dictree_set_bin(dictree *dict, char *key, int key_length,
	char *value, int value_length){
	if (key == NULL){
		fprintf(stderr, "[error] dictree_set_bin: key == NULL\n");
		return;
	}
	if (value == NULL){
		fprintf(stderr, "[error] dictree_set_bin: value == NULL\n");
		return;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] dictree_set_bin: dict->root == NULL\n");
		return;
	}
	int i;
	char j;
	dict_node *node = NULL;
	dict_node *node_next = NULL;
	dict_node *node_last = NULL;
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
		str_reset(node->key);
		free(node->key);
		node->key = NULL;
	}
	if (node->value != NULL){
		str_reset(node->value);
		free(node->value);
		node->value = NULL;
	}
	node->key = new_str_p_from_bin(key, key_length);
	node->value = new_str_p_from_bin(value, value_length);
}

void dictree_set_char(dictree *dict, char *key, char *value){
	int key_length = char_len(key);
	int value_length = char_len(value);
	dictree_set_bin(dict, key, key_length, value, value_length);
}

void dictree_set_str(dictree *dict, str *key, str *value){
	dictree_set_bin(dict, key->value, key->length, value->value, value->length);
}

void dictree_set_char_str(dictree *dict, char *key, str *value){
	int key_length = char_len(key);
	dictree_set_bin(dict, key, key_length, value->value, value->length);
}

dict_node *dictree_get_dict_node(dictree *dict, char *key, int key_value){
	if (key == NULL){
		fprintf(stderr, "[error] dictree_get_dict_node: key == NULL\n");
		return NULL;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] dictree_get_dict_node: dict->root == NULL\n");
		return NULL;
	}
	int i;
	dict_node *node = dict->root;
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
	int key_length = char_len(key);
	return dictree_get_str_from_bin(dict, key, key_length);
}

str dictree_get_str_from_str(dictree *dict, str *key){
	return dictree_get_str_from_bin(dict, key->value, key->length);
}

int dictree_remove_from_bin(dictree *dict, char *key, int key_length){
	//return error
	dict_node *node = dictree_get_dict_node(dict, key, key_length);
	if (node == NULL || (node->key == NULL && node->value == NULL)){
		fprintf(stderr, "[error] dictree_remove_from_bin: %s not in dict", key);
		return -1;
	}
	if (dict->root == NULL){
		fprintf(stderr, "[error] dictree_remove_from_bin: dict->root == NULL\n");
		return -2;
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
	return 0;
}

int dictree_remove_from_char(dictree *dict, char *key){
	int key_length = char_len(key);
	return dictree_remove_from_bin(dict, key, key_length);
}

int dictree_remove_from_str(dictree *dict, str *key){
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
	if (dict->root == NULL){
		fprintf(stderr, "[error] print_dictree: dict->root == NULL\n");
		return;
	}
	printf("{");
	print_dict_node(dict->root);
	printf("}");
	printf("\n");
}