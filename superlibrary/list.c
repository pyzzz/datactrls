#define DATA_TYPE_NULL 0
#define DATA_TYPE_INT 10
#define DATA_TYPE_STR 20

struct list_data{
	int num;
	str *string;
	byte type;
};
typedef struct list_data list_data;

struct list_child{
	list_data *data;
	struct list_child *prev;
	struct list_child *next;
};
typedef struct list_child list_child;

struct list{
	int length;
	list_child *start;
	list_child *end;
};
typedef struct list list;

list_data *new_list_data_p(){
	list_data *data = (list_data*) malloc(sizeof(list_data));
	data->num = -1;
	data->string = NULL;
	data->type = DATA_TYPE_NULL;
	return data;
}

void list_data_set_int(list_data *data, int num){
	str_reset(data->string);
	free(data->string);
	data->string = NULL;
	data->num = num;
	data->type = DATA_TYPE_INT;
}

void list_data_set_str(list_data *data, str *string_p){
	str_reset(data->string);
	free(data->string);
	data->string = NULL;
	data->num = -1;
	data->string = new_str_p_from_copy(string_p);
	data->type = DATA_TYPE_STR;
}

void list_data_set_bin(list_data *data, char *value, int value_length){
	str_reset(data->string);
	free(data->string);
	data->string = NULL;
	data->num = -1;
	data->string = new_str_p_from_bin(value, value_length);
	data->type = DATA_TYPE_STR;
}

void list_data_set_char(list_data *data, char *value){
	str_reset(data->string);
	free(data->string);
	data->string = NULL;
	data->num = -1;
	data->string = new_str_p(value);
	data->type = DATA_TYPE_STR;
}

void list_data_reset(list_data *data){
	if (data == NULL){
		return;
	}
	str_reset(data->string);
	free(data->string);
	data->string = NULL;
	data->num = -1;
	data->type = DATA_TYPE_NULL;
}

void list_child_reset(list_child *m){
	if (m == NULL){
		return;
	}
	list_data_reset(m->data);
	free(m->data);
	m->data = NULL;
	m->prev = NULL;
	m->next = NULL;
}

void list_insert(list *l, list_data *data, int index){
	list_child *m;
	list_child *fault;
	int i;
	if (index < 0){
		printf("[error] list_insert: index [%d] < 0\n", index);
		return;
	}
	m = (list_child*) malloc(sizeof(list_child));
	m->data = data;
	if (l->length == 0){
		//if not list
		m->prev = NULL;
		m->next = NULL;
		l->start = m;
		l->end = m;
	}
	else if (index == 0){
		//if insert to list start
		m->prev = NULL;
		m->next = l->start;
		m->next->prev = m;
		l->start = m;
	}
	else if (index >= l->length){
		//if insert to list end
		m->prev = l->end;
		m->next = NULL;
		m->prev->next = m;
		l->end = m;
	}
	else{
		fault = l->start;
		for (i=0; i<index; i++){
			fault = fault->next;
		}
		//printf("insert before %d\n", fault->data->num);
		m->prev = fault->prev;
		m->next = fault;
		fault->prev->next = m;
		fault->prev = m;
	}
	l->length += 1;
}

void list_insert_int(list *l, int num, int index){
	list_data *data = new_list_data_p();
	list_data_set_int(data, num);
	list_insert(l, data, index);
}

void list_insert_str(list *l, str *string_p, int index){
	list_data *data = new_list_data_p();
	list_data_set_str(data, string_p);
	list_insert(l, data, index);
}

void list_insert_char(list *l, char *value, int index){
	list_data *data = new_list_data_p();
	list_data_set_char(data, value);
	list_insert(l, data, index);
}

void list_insert_bin(list *l, char *value, int value_length, int index){
	list_data *data = new_list_data_p();
	list_data_set_bin(data, value, value_length);
	list_insert(l, data, index);
}

void list_append(list *l, list_data *data){
	list_insert(l, data, l->length);
}

void list_append_int(list *l, int num){
	list_insert_int(l, num, l->length);
}

void list_append_str(list *l, str *string_p){
	list_insert_str(l, string_p, l->length);
}

void list_append_char(list *l, char *value){
	list_insert_char(l, value, l->length);
}

void list_append_bin(list *l, char *value, int value_length){
	list_insert_bin(l, value, value_length, l->length);
}

int list_find_int(list *l, int num){
	//return index
	list_child *m = NULL;
	int i;
	m = l->start;
	for (i=0; i<l->length; i++){
		if (m->data->type == DATA_TYPE_INT && m->data->num == num){
			return i;
		}
		m = m->next;
	}
	return -1;
}

int list_find_str(list *l, str *string_p){
	//return index
	list_child *m = NULL;
	int i;
	m = l->start;
	for (i=0; i<l->length; i++){
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal(m->data->string, string_p) == 1){
				return i;
			}
		}
		m = m->next;
	}
	return -1;
}

int list_find_char(list *l, char *value){
	//return index
	list_child *m = NULL;
	int i;
	m = l->start;
	for (i=0; i<l->length; i++){
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal_char(m->data->string, value) == 1){
				return i;
			}
		}
		m = m->next;
	}
	return -1;
}

void list_remove_child(list *l, list_child *m){
	/*byte in_list = 0;
	list_child *n = NULL;
	n = l->start;
	while (1){
		if (n == NULL){
			break;
		}
		if (n == m){
			in_list = 1;
			break;
		}
		n = n->next;
	}
	if (in_list == 0){
		fprintf(stderr, "[error] list_remove_child: list_child not in list\n");
	}*/
	if (m->prev == NULL && m->next == NULL){
		l->start = NULL;
		l->end = NULL;
	}
	else if (m->prev == NULL && m->next != NULL){
		l->start = m->next;
		m->next->prev = NULL;
	}
	else if (m->prev != NULL && m->next == NULL){
		l->end = m->prev;
		m->prev->next = NULL;
	}
	else{
		m->prev->next = m->next;
		m->next->prev = m->prev;
	}
	l->length -= 1;
	list_child_reset(m);
	free(m);
	m = NULL;
}

int list_remove(list *l, int index){
	//remove from index, return bool
	if (index < 0){
		fprintf(stderr, "[error] list_remove: index [%d] < 0\n", index);
		return 0;
	}
	else if (index >= l->length){
		fprintf(stderr, "[error] list_remove: index [%d] out of range\n", index);
		return 0;
	}
	list_child *m = NULL;
	int i;
	m = l->start;
	for (i=0; i<index; i++){
		m = m->next;
	}
	list_remove_child(l, m);
	return 1;
}

int list_remove_int(list *l, int num){
	//return bool
	list_child *m = NULL;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		if (m->data->type == DATA_TYPE_INT && m->data->num == num){
			list_remove_child(l, m);
			return 1;
		}
		m = m->next;
	}
	fprintf(stderr, "[error] list_remove_int: %d not in list\n", num);
	return 0;
}

int list_remove_str(list *l, str *string_p){
	//return bool
	list_child *m = NULL;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal(m->data->string, string_p) == 1){
				list_remove_child(l, m);
				return 1;
			}
		}
		m = m->next;
	}
	fprintf(stderr, "[error] list_remove_str: \"%s\" not in list\n",
		string_p->value);
	return 0;
}

int list_remove_char(list *l, char *value){
	//return bool
	list_child *m = NULL;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal_char(m->data->string, value) == 1){
				list_remove_child(l, m);
				return 1;
			}
		}
		m = m->next;
	}
	fprintf(stderr, "[error] list_remove_char: \"%s\" not in list\n", value);
	return 0;
}

int list_remove_all_int(list *l, int num){
	//return count
	list_child *m = NULL;
	list_child *m_next = NULL;
	int count = 0;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		m_next = m->next;
		if (m->data->type == DATA_TYPE_INT && m->data->num == num){
			list_remove_child(l, m);
			count += 1;
		}
		m = m_next;
	}
	return count;
}

int list_remove_all_str(list *l, str *string_p){
	//return count
	list_child *m = NULL;
	list_child *m_next = NULL;
	int count = 0;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		m_next = m->next;
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal(m->data->string, string_p) == 1){
				list_remove_child(l, m);
				count += 1;
			}
		}
		m = m_next;
	}
	return count;
}

int list_remove_all_char(list *l, char *value){
	//return count
	list_child *m = NULL;
	list_child *m_next = NULL;
	int count = 0;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		m_next = m->next;
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal_char(m->data->string, value) == 1){
				list_remove_child(l, m);
				count += 1;
			}
		}
		m = m_next;
	}
	return count;
}

list_data *list_get(list *l, int index){
	//return value address
	if (index < 0){
		fprintf(stderr, "[error] list_get: index [%d] < 0\n", index);
		return NULL;
	}
	else if (index >= l->length){
		fprintf(stderr, "[error] list_get: index [%d] out of range\n", index);
		return NULL;
	}
	list_child *m = NULL;
	int i = 0;
	m = l->start;
	for (i=0; i<index; i++){
		m = m->next;
	}
	return m->data;
}

int list_get_int(list *l, int index){
	//return num
	list_data *data = NULL;
	data = list_get(l, index);
	if (data == NULL){
		return -1;
	}
	else if (data->type != DATA_TYPE_INT){
		fprintf(stderr, "[error] list_get_int: data type not int\n");
		return -1;
	}
	return data->num;
}

str list_get_str(list *l, int index){
	//return str*
	list_data *data = NULL;
	str result = new_str("");
	data = list_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr, "[error] list_get_str: data type not str\n");
		return result;
	}
	str_set_bin(&result, data->string->value, data->string->length);
	return result;
}

char *list_get_char(list *l, int index){
	//return char*
	list_data *data = NULL;
	char *result = malloc(sizeof(char)*1);
	result[0] = '\x00';
	data = list_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr, "[error] list_get_char: data type not str\n");
		return result;
	}
	free(result);
	result = NULL;
	result = char_copy(data->string->value, data->string->length);
	return result;
}

void list_reset(list *l){
	while (l->start != NULL){
		list_remove_child(l, l->start);
	}
}

void print_list(list *l){
	list_child *m = NULL;
	printf("list length: %d\n", l->length);
	m = l->start;
	printf("[");
	while (1){
		if (m == NULL){
			break;
		}
		if (m->data->type == DATA_TYPE_INT){
			printf("%d, ", m->data->num);
		}
		else if (m->data->type == DATA_TYPE_STR){
			print_str_verbose(m->data->string);
			printf(", ");
		}
		else{
			printf("NULL, ");
		}
		m = m->next;
	}
	printf("]\n");
}

list new_list(){
	list l;
	l.length = 0;
	l.start = NULL;
	l.end = NULL;
	return l;
}

list new_list_from_range(int start, int end){
	list l = new_list();
	int i;
	for (i=start; i<end; i++){
		list_append_int(&l, i);
	}
	return l;
}

list new_list_from_split_bin(char *value, int value_length,
	char *split_key, char skip_space){
	list l = new_list();
	str string = new_str("");
	int split_key_length = char_len(split_key);
	char *word = malloc(sizeof(char)*(value_length+1));
	int i;
	int word_i = 0;
	int split_key_i = 0;
	char j;
	for (i=0; i<value_length; i++){
		j = value[i];
		//if ((skip_space != 0) && (j == ' ' || j == '\t')){
		//	continue;
		//}
		word[word_i] = j;
		//word[word_i+1] = '\x00';
		word_i += 1;
		if (j == split_key[split_key_i]){
			split_key_i += 1;
		}
		else{
			split_key_i = 0;
		}
		if (split_key[split_key_i] == '\x00'){
			str_set_bin(&string, word, word_i-split_key_length);
			if (skip_space != 0){
				str_strip(&string);
			}
			list_append_str(&l, &string);
			word_i = 0;
			split_key_i = 0;
			word[0] = '\x00';
		}
		//printf("j %c split_key[split_key_i] %c split_key_i %d\n",
		//	j, split_key[split_key_i], split_key_i);
	}
	str_set_bin(&string, word, word_i);
	if (skip_space != 0){
		str_strip(&string);
	}
	list_append_str(&l, &string);
	free(word);
	word = NULL;
	return l;
}

list new_list_from_split_str(str *string_p, char *split_key){
	return new_list_from_split_bin(string_p->value, string_p->length, split_key, 0);
}

list new_list_from_split_str_skip_space(str *string_p, char *split_key){
	return new_list_from_split_bin(string_p->value, string_p->length, split_key, 1);
}

list new_list_from_split_char(char *value, char *split_key){
	int value_length = char_len(value);
	return new_list_from_split_bin(value, value_length, split_key, 0);
}

list new_list_from_split_char_skip_space(char *value, char *split_key){
	int value_length = char_len(value);
	return new_list_from_split_bin(value, value_length, split_key, 1);
}

list new_list_from_split_bin_space(char *value, int value_length){
	list l = new_list();
	char *word = malloc(sizeof(char)*(value_length+1));
	int i;
	int word_i = 0;
	char j;
	for (i=0; i<value_length; i++){
		j = value[i];
		if (j == ' ' || j == '\t' || j == '\x00'){
			word[word_i] = '\x00';
			if (word[0] != '\x00'){
				list_append_bin(&l, word, word_i);
				word[0] = '\x00';
			}
			word_i = 0;
		}
		else{
			word[word_i] = j;
			word_i += 1;
		}
		//printf("j %d\n", j);
	}
	if (word[0] != '\x00'){
		list_append_bin(&l, word, word_i);
	}
	free(word);
	word = NULL;
	return l;
}

list new_list_from_split_str_space(str *string_p){
	return new_list_from_split_bin_space(string_p->value, string_p->length);
}

list new_list_from_split_char_space(char *value){
	int value_length = char_len(value);
	return new_list_from_split_bin_space(value, value_length);
}
