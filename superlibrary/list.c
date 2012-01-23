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

void list_data_set_str(list_data *data, str *string){
	str_reset(data->string);
	free(data->string);
	data->string = NULL;
	data->num = -1;
	data->string = new_str_p_from_copy(string);
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

int list_find_str(list *l, str *string){
	//return index
	list_child *m = NULL;
	int i;
	m = l->start;
	for (i=0; i<l->length; i++){
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
	list_child_reset(m);
	free(m);
	m = NULL;
	l->length -= 1;
	return 1;
}

int list_remove_int(list *l, int num){
	//return bool
	int index = list_find_int(l, num);
	if (index < 0){
		return 0;
	}
	list_remove(l, index);
	return 1;
}

int list_remove_str(list *l, str *string){
	//return bool
	int index = list_find_str(l, string);
	if (index < 0){
		return 0;
	}
	list_remove(l, index);
	return 1;
}

int list_remove_char(list *l, char *value){
	//return bool
	int index = list_find_char(l, value);
	if (index < 0){
		return 0;
	}
	list_remove(l, index);
	return 1;
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
			printf("\"%s\", ", m->data->string->value);
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
