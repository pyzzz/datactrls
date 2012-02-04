//list (chain)

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
		fprintf(stderr, "[error] list_insert: index < 0 [%d]\n", index);
		return;
	}
	m = malloc(sizeof(list_child));
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

void list_insert_str(list *l, str *string, int index){
	list_data *data = new_list_data_p();
	list_data_set_str(data, string);
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

void list_append_str(list *l, str *string){
	list_insert_str(l, string, l->length);
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
	int length = l->length;
	m = l->start;
	for (i=0; i<length; i++){
		if (m->data->type == DATA_TYPE_INT && m->data->num == num){
			return i;
		}
		m = m->next;
	}
	return -1;
}

int list_find_str(list *l, str *string){
	//return index
	list_child *m = NULL;
	int i;
	int length = l->length;
	m = l->start;
	for (i=0; i<length; i++){
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal(m->data->string, string) == 1){
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
	int length = l->length;
	m = l->start;
	for (i=0; i<length; i++){
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
	#if NULL_ARG_CHECK
	if (m == NULL){
		fprintf(stderr, "[error] list_remove_child: m == NULL\n");
		return;
	}
	#endif
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
		fprintf(stderr, "[error] list_remove: index < 0 [%d]\n", index);
		return 0;
	}
	if (index >= l->length){
		fprintf(stderr, "[error] list_remove: index out of range [%d]\n", index);
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

byte list_remove_int(list *l, int num){
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
	fprintf(stderr, "[error] list_remove_int: int not in list [%d]\n", num);
	return 0;
}

byte list_remove_str(list *l, str *string){
	//return bool
	list_child *m = NULL;
	m = l->start;
	while (1){
		if (m == NULL){
			break;
		}
		if (m->data->type == DATA_TYPE_STR){
			if (str_equal(m->data->string, string) == 1){
				list_remove_child(l, m);
				return 1;
			}
		}
		m = m->next;
	}
	fprintf(stderr, "[error] list_remove_str: str not in list [\"%s\"]\n",
		string->value);
	return 0;
}

byte list_remove_char(list *l, char *value){
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
	fprintf(stderr, "[error] list_remove_char: char not in list [\"%s\"]\n",
		value);
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

int list_remove_all_str(list *l, str *string){
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
			if (str_equal(m->data->string, string) == 1){
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
		fprintf(stderr, "[error] list_get: index < 0 [%d]\n", index);
		return NULL;
	}
	else if (index >= l->length){
		fprintf(stderr, "[error] list_get: index out of range [%d]\n", index);
		return NULL;
	}
	int i;
	list_child *m = NULL;
	if (index <= (l->length / 2)){
		m = l->start;
		for (i=0; i<index; i++){
			m = m->next;
		}
	}
	else{
		m = l->end;
		for (i=(l->length-1-index); i>0; i--){
			m = m->prev;
		}
	}
	return m->data;
}

int new_int_from_list_get(list *l, int index){
	//return num
	list_data *data = NULL;
	data = list_get(l, index);
	if (data == NULL){
		return -1;
	}
	else if (data->type != DATA_TYPE_INT){
		fprintf(stderr,
			"[error] new_int_from_list_get: data type not int [index:%d]\n",
			index);
		return -1;
	}
	return data->num;
}

str new_str_from_list_get(list *l, int index){
	list_data *data = NULL;
	str result = new_str(NULL);
	data = list_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] new_str_from_list_get: data type not str [index:%d]\n",
			index);
		return result;
	}
	str_set_bin(&result, data->string->value, data->string->length);
	return result;
}

str *new_str_p_from_list_get(list *l, int index){
	list_data *data = NULL;
	str *result = new_str_p(NULL);
	data = list_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] new_str_p_from_list_get: data type not str [index:%d]\n",
			index);
		return result;
	}
	str_set_bin(result, data->string->value, data->string->length);
	return result;
}

void str_set_list_get(str *string, list *l, int index){
	list_data *data = NULL;
	data = list_get(l, index);
	if (data == NULL){
		return;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] str_set_list_get: data type not str [index:%d]\n",
			index);
		return;
	}
	str_set_bin(string, data->string->value, data->string->length);
}

char *new_char_from_list_get(list *l, int index){
	//return char*
	list_data *data = NULL;
	char *result = NULL;
	data = list_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] new_char_from_list_get: data type not str [index:%d]\n",
			index);
		return result;
	}
	result = malloc(sizeof(char)*(data->string->length+1));
	memcpy(result, data->string->value, data->string->length);
	return result;
}

void list_reset(list *l){
	while (l->start != NULL){
		list_remove_child(l, l->start);
	}
}

void print_list(list *l){
	list_child *m = NULL;
	m = l->start;
	printf("list[%d] [", l->length);
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

list *new_list_p(){
	list *l = malloc(sizeof(list));
	l->length = 0;
	l->start = NULL;
	l->end = NULL;
	return l;
}

void list_append_range(list *l, int start, int end){
	register int i;
	for (i=start; i<end; i++){
		list_append_int(l, i);
	}
}

void list_append_range_with_step(list *l, int start, int end, int step){
	register int i;
	for (i=start; i<end; i+=step){
		list_append_int(l, i);
	}
}

list new_list_from_range(int start, int end){
	list l = new_list();
	list_append_range(&l, start, end);
	return l;
}

list *new_list_p_from_range(int start, int end){
	list *l = new_list_p();
	list_append_range(l, start, end);
	return l;
}

list new_list_from_range_with_step(int start, int end, int step){
	list l = new_list();
	list_append_range_with_step(&l, start, end, step);
	return l;
}

list *new_list_p_from_range_with_step(int start, int end, int step){
	list *l = new_list_p();
	list_append_range_with_step(l, start, end, step);
	return l;
}

void list_append_split_bin(list *l, char *value, int value_length,
	char *split_key, byte skip_space){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] list_append_split_bin: value == NULL\n");
		return;
	}
	if (split_key == NULL){
		fprintf(stderr, "[error] list_append_split_bin: split_key == NULL\n");
		return;
	}
	#endif
	str *string = new_str_p(NULL);
	int split_key_length = strlen(split_key);
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
			if (skip_space != 0){
				str_set_bin(string, word, word_i-split_key_length);
				str_strip(string);
				list_append_str(l, string);
			}
			else{
				list_append_bin(l, word, word_i-split_key_length);
			}
			word_i = 0;
			split_key_i = 0;
			word[0] = '\x00';
		}
		//printf("j %c split_key[split_key_i] %c split_key_i %d\n",
		//	j, split_key[split_key_i], split_key_i);
	}
	if (skip_space != 0){
		str_set_bin(string, word, word_i);
		str_strip(string);
		list_append_str(l, string);
	}
	else{
		list_append_bin(l, word, word_i);
	}
	str_reset(string);
	free(word);
	free(string);
	word = NULL;
	string = NULL;
}

void list_append_split_str(list *l, str *string, char *split_key){
	list_append_split_bin(l, string->value, string->length, split_key, 0);
}

void list_append_split_str_skip_space(list *l, str *string, char *split_key){
	list_append_split_bin(l, string->value, string->length, split_key, 1);
}

void list_append_split_char(list *l, char *value, char *split_key){
	list_append_split_bin(l, value, strlen(value), split_key, 0);
}

void list_append_split_char_skip_space(list *l, char *value, char *split_key){
	list_append_split_bin(l, value, strlen(value), split_key, 1);
}

list new_list_from_split_bin(char *value, int value_length,
	char *split_key, byte skip_space){
	list l = new_list();
	list_append_split_bin(&l, value, value_length, split_key, skip_space);
	return l;
}

list *new_list_p_from_split_bin(char *value, int value_length,
	char *split_key, byte skip_space){
	list *l = new_list_p();
	list_append_split_bin(l, value, value_length, split_key, skip_space);
	return l;
}

list new_list_from_split_str(str *string, char *split_key){
	list l = new_list();
	list_append_split_bin(&l, string->value, string->length, split_key, 0);
	return l;
}

list *new_list_p_from_split_str(str *string, char *split_key){
	list *l = new_list_p();
	list_append_split_bin(l, string->value, string->length, split_key, 0);
	return l;
}

list new_list_from_split_str_skip_space(str *string, char *split_key){
	list l = new_list();
	list_append_split_bin(&l, string->value, string->length, split_key, 1);
	return l;
}

list *new_list_p_from_split_str_skip_space(str *string, char *split_key){
	list *l = new_list_p();
	list_append_split_bin(l, string->value, string->length, split_key, 1);
	return l;
}

list new_list_from_split_char(char *value, char *split_key){
	list l = new_list();
	list_append_split_bin(&l, value, strlen(value), split_key, 0);
	return l;
}

list *new_list_p_from_split_char(char *value, char *split_key){
	list *l = new_list_p();
	list_append_split_bin(l, value, strlen(value), split_key, 0);
	return l;
}

list new_list_from_split_char_skip_space(char *value, char *split_key){
	list l = new_list();
	list_append_split_bin(&l, value, strlen(value), split_key, 1);
	return l;
}

list *new_list_p_from_split_char_skip_space(char *value, char *split_key){
	list *l = new_list_p();
	list_append_split_bin(l, value, strlen(value), split_key, 1);
	return l;
}

void list_append_split_bin_space(list *l, char *value, int value_length){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] list_append_split_bin_space: value == NULL\n");
		return;
	}
	#endif
	char *word = malloc(sizeof(char)*(value_length+1));
	int i;
	int word_i = 0;
	char j;
	for (i=0; i<value_length; i++){
		j = value[i];
		if (j == ' ' || j == '\t' || j == '\x00'){
			word[word_i] = '\x00';
			if (word[0] != '\x00'){
				list_append_bin(l, word, word_i);
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
		list_append_bin(l, word, word_i);
	}
	free(word);
	word = NULL;
}

list new_list_from_split_bin_space(char *value, int value_length){
	list l = new_list();
	list_append_split_bin_space(&l, value, value_length);
	return l;
}

list *new_list_p_from_split_bin_space(char *value, int value_length){
	list *l = new_list_p();
	list_append_split_bin_space(l, value, value_length);
	return l;
}

list new_list_from_split_str_space(str *string){
	list l = new_list();
	list_append_split_bin_space(&l, string->value, string->length);
	return l;
}

list *new_list_p_from_split_str_space(str *string){
	list *l = new_list_p();
	list_append_split_bin_space(l, string->value, string->length);
	return l;
}

list new_list_from_split_char_space(char *value){
	list l = new_list();
	list_append_split_bin_space(&l, value, strlen(value));
	return l;
}

list *new_list_p_from_split_char_space(char *value){
	list *l = new_list_p();
	list_append_split_bin_space(l, value, strlen(value));
	return l;
}