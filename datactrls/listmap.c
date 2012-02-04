//list (map)

struct listmap{
	list_data **map;
	int length;
	int length_malloc;
	int poll_size;
};
typedef struct listmap listmap;

listmap new_listmap(){
	listmap l;
	l.map = NULL;
	l.length = 0;
	l.length_malloc = 0;
	l.poll_size = LISTMAP_POLL_SIZE;
	return l;
}

listmap *new_listmap_p(){
	listmap *l = malloc(sizeof(listmap));
	l->map = NULL;
	l->length = 0;
	l->length_malloc = 0;
	l->poll_size = LISTMAP_POLL_SIZE;
	return l;
}

void listmap_set_poll_size(listmap *l, int poll_size){
	if (poll_size < 1){
		fprintf(stderr,
		"[error] listmap_set_poll_size: poll_size < 1 [%d]",
		poll_size);
	}
	else{
		l->poll_size = poll_size;
	}
}

listmap new_listmap_with_poll_size(int poll_size){
	listmap l = new_listmap();
	listmap_set_poll_size(&l, poll_size);
	return l;
}

listmap *new_listmap_p_with_poll_size(int poll_size){
	listmap *l = new_listmap_p();
	listmap_set_poll_size(l, poll_size);
	return l;
}

void listmap_malloc(listmap *l, int poll_size){
	int i;
	int length_malloc;
	/*printf("listmap_malloc real size(%d) old size(%d) new size(%d)\n",
		l->length, l->length_malloc, l->length_malloc+poll_size);*/
	list_data **map_old = l->map;
	l->length_malloc += poll_size;
	l->map = malloc(sizeof(list_data*)*l->length_malloc);
	if (map_old != NULL){
		memcpy(l->map, map_old, sizeof(list_data*)*l->length);
	}
	length_malloc = l->length_malloc;
	for (i=l->length; i<length_malloc; i++){
		l->map[i] = NULL;
	}
	free(map_old);
}

void listmap_append(listmap *l, list_data *data){
	if (l->length >= l->length_malloc){
		listmap_malloc(l, l->poll_size);
	}
	l->map[l->length] = data;
	l->length += 1;
}

void listmap_append_int(listmap *l, int num){
	list_data *data = new_list_data_p();
	list_data_set_int(data, num);
	listmap_append(l, data);
}

void listmap_append_str(listmap *l, str *string){
	list_data *data = new_list_data_p();
	list_data_set_str(data, string);
	listmap_append(l, data);
}

void listmap_append_char(listmap *l, char *value){
	list_data *data = new_list_data_p();
	list_data_set_char(data, value);
	listmap_append(l, data);
}

void listmap_append_bin(listmap *l, char *value, int value_length){
	list_data *data = new_list_data_p();
	list_data_set_bin(data, value, value_length);
	listmap_append(l, data);
}

void listmap_append_range(listmap *l, int start, int end){
	register int i;
	listmap_malloc(l, end-start);
	for (i=start; i<end; i++){
		listmap_append_int(l, i);
	}
}

void listmap_append_range_with_step(listmap *l, int start, int end, int step){
	register int i;
	listmap_malloc(l, end-start);
	for (i=start; i<end; i+=step){
		listmap_append_int(l, i);
	}
}

listmap new_listmap_from_range(int start, int end){
	listmap l = new_listmap();
	listmap_append_range(&l, start, end);
	return l;
}

listmap *new_listmap_p_from_range(int start, int end){
	listmap *l = new_listmap_p();
	listmap_append_range(l, start, end);
	return l;
}

listmap new_listmap_from_range_with_step(int start, int end, int step){
	listmap l = new_listmap();
	listmap_append_range_with_step(&l, start, end, step);
	return l;
}

listmap *new_listmap_p_from_range_with_step(int start, int end, int step){
	listmap *l = new_listmap_p();
	listmap_append_range_with_step(l, start, end, step);
	return l;
}

void listmap_malloc_insert(listmap *l, int poll_size, int index, list_data *data){
	int i;
	int length_malloc;
	list_data **map_old = l->map;
	if (l->length >= l->length_malloc){
		/*printf(
			"listmap_malloc_insert real size(%d) old size(%d) new size(%d)\n",
			l->length, l->length_malloc, l->length_malloc+poll_size);*/
		l->length_malloc += poll_size;
	}
	l->map = malloc(sizeof(list_data*)*l->length_malloc);
	if (map_old != NULL){
		memcpy(l->map, map_old, sizeof(list_data*)*index);
		memcpy(l->map+index+1, map_old+index, sizeof(list_data*)*(l->length-index));
	}
	l->map[index] = data;
	l->length += 1;
	length_malloc = l->length_malloc;
	for (i=l->length; i<length_malloc; i++){
		l->map[i] = NULL;
	}
	free(map_old);
}

void listmap_insert(listmap *l, list_data *data, int index){
	if (index < 0){
		fprintf(stderr, "[error] listmap_insert: index < 0 [%d]\n", index);
		return;
	}
	if (index >= l->length){
		index = l->length;
	}
	listmap_malloc_insert(l, l->poll_size, index, data);
}

void listmap_insert_int(listmap *l, int num, int index){
	list_data *data = new_list_data_p();
	list_data_set_int(data, num);
	listmap_insert(l, data, index);
}

void listmap_insert_str(listmap *l, str *string, int index){
	list_data *data = new_list_data_p();
	list_data_set_str(data, string);
	listmap_insert(l, data, index);
}

void listmap_insert_char(listmap *l, char *value, int index){
	list_data *data = new_list_data_p();
	list_data_set_char(data, value);
	listmap_insert(l, data, index);
}

void listmap_insert_bin(listmap *l, char *value, int value_length, int index){
	list_data *data = new_list_data_p();
	list_data_set_bin(data, value, value_length);
	listmap_insert(l, data, index);
}

int listmap_find_int(listmap *l, int num){
	//return index
	int i;
	int length = l->length;
	for (i=0; i<length; i++){
		if (l->map[i]->type == DATA_TYPE_INT && l->map[i]->num == num){
			return i;
		}
	}
	return -1;
}

int listmap_find_str(listmap *l, str *string){
	//return index
	int i;
	int length = l->length;
	for (i=0; i<length; i++){
		if (l->map[i]->type == DATA_TYPE_STR){
			if (str_equal(l->map[i]->string, string) == 1){
				return i;
			}
		}
	}
	return -1;
}

int listmap_find_char(listmap *l, char *value){
	//return index
	int i;
	int length = l->length;
	for (i=0; i<length; i++){
		if (l->map[i]->type == DATA_TYPE_STR){
			if (str_equal_char(l->map[i]->string, value) == 1){
				return i;
			}
		}
	}
	return -1;
}

byte listmap_remove(listmap *l, int index){
	//remove from index, return bool
	if (index < 0){
		fprintf(stderr, "[error] listmap_remove: index < 0 [%d]\n", index);
		return 0;
	}
	if (index >= l->length){
		fprintf(stderr,
			"[error] listmap_remove: index out of range [%d]\n", index);
		return 0;
	}
	list_data_reset(l->map[index]);
	free(l->map[index]);
	//source and destination overlap in memcpy @ valgrind
	//memcpy(l->map+index, l->map+index+1, sizeof(list_data*)*(l->length-index));
	int i;
	int length = l->length;
	for (i=index+1; i<length; i++){
		l->map[i-1] = l->map[i];
	}
	l->map[l->length] = NULL;
	l->length -= 1;
	return 1;
}

byte listmap_remove_int(listmap *l, int num){
	//return bool
	int index = listmap_find_int(l, num);
	if (index != -1){
		listmap_remove(l, index);
		return 1;
	}
	//fprintf(stderr, "[error] listmap_remove_int: int not in list [%d]\n", num);
	return 0;
}

byte listmap_remove_str(listmap *l, str *string){
	//return bool
	int index = listmap_find_str(l, string);
	if (index != -1){
		listmap_remove(l, index);
		return 1;
	}
	/*fprintf(stderr, "[error] list_remove_str: str not in list [%s]\n",
		string->value);*/
	return 0;
}

byte listmap_remove_char(listmap *l, char *value){
	//return bool
	int index = listmap_find_char(l, value);
	if (index != -1){
		listmap_remove(l, index);
		return 1;
	}
	/*fprintf(stderr, "[error] listmap_remove_char: char not in list [\"%s\"]\n",
		value);*/
	return 0;
}

int listmap_remove_all_int(listmap *l, int num){
	//return count
	int count = 0;
	while(listmap_remove_int(l, num)){
		count += 1;
	}
	return count;
}

int listmap_remove_all_str(listmap *l, str *string){
	//return count
	int count = 0;
	while(listmap_remove_str(l, string)){
		count += 1;
	}
	return count;
}

int listmap_remove_all_char(listmap *l, char *value){
	//return count
	int count = 0;
	while(listmap_remove_char(l, value)){
		count += 1;
	}
	return count;
}

void listmap_reset(listmap *l){
	int i;
	int length = l->length;
	for (i=0; i<length; i++){
		list_data_reset(l->map[i]);
		free(l->map[i]);
	}
	free(l->map);
	l->map = NULL;
	l->length = 0;
	l->length_malloc = 0;
}

list_data *listmap_get(listmap *l, int index){
	//return value address
	if (index < 0){
		fprintf(stderr, "[error] listmap_get: index < 0 [%d]\n", index);
		return NULL;
	}
	else if (index >= l->length){
		fprintf(stderr,
			"[error] listmap_get: index out of range [%d]\n", index);
		return NULL;
	}
	return l->map[index];
}

int new_int_from_listmap_get(listmap *l, int index){
	//return num
	list_data *data = NULL;
	data = listmap_get(l, index);
	if (data == NULL){
		return -1;
	}
	else if (data->type != DATA_TYPE_INT){
		fprintf(stderr,
			"[error] new_int_from_listmap_get: data type not int [index:%d]\n",
			index);
		return -1;
	}
	return data->num;
}

str new_str_from_listmap_get(listmap *l, int index){
	list_data *data = NULL;
	str result = new_str(NULL);
	data = listmap_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] new_str_from_listmap_get: data type not str [index:%d]\n",
			index);
		return result;
	}
	str_set_bin(&result, data->string->value, data->string->length);
	return result;
}

str *new_str_p_from_listmap_get(listmap *l, int index){
	list_data *data = NULL;
	str *result = new_str_p(NULL);
	data = listmap_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
		"[error] new_str_p_from_listmap_get: data type not str [index:%d]\n",
		index);
		return result;
	}
	str_set_bin(result, data->string->value, data->string->length);
	return result;
}

void str_set_listmap_get(str *string, listmap *l, int index){
	list_data *data = NULL;
	data = listmap_get(l, index);
	if (data == NULL){
		return;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] str_set_listmap_get: data type not str [index:%d]\n",
			index);
		return;
	}
	str_set_bin(string, data->string->value, data->string->length);
}

char *new_char_from_listmap_get(listmap *l, int index){
	//return char*
	list_data *data = NULL;
	char *result = NULL;
	data = listmap_get(l, index);
	if (data == NULL){
		return result;
	}
	else if (data->type != DATA_TYPE_STR){
		fprintf(stderr,
			"[error] new_char_from_listmap_get: data type not str [index:%d]\n",
			index);
		return result;
	}
	result = malloc(sizeof(char)*(data->string->length+1));
	memcpy(result, data->string->value, data->string->length);
	return result;
}

void print_listmap(listmap *l){
	int i;
	int length = l->length;
	printf("listmap[%d] [", l->length);
	for (i=0; i<length; i++){
		//fprintf(stderr, "%d\n", i);
		if (l->map[i]->type == DATA_TYPE_INT){
			printf("%d, ", l->map[i]->num);
		}
		else if (l->map[i]->type == DATA_TYPE_STR){
			print_str_verbose(l->map[i]->string);
			printf(", ");
		}
		else{
			printf("NULL, ");
		}
	}
	printf("]\n");
}

void listmap_append_split_bin(listmap *l, char *value, int value_length,
	char *split_key, byte skip_space){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] listmap_append_split_bin: value == NULL\n");
		return;
	}
	if (split_key == NULL){
		fprintf(stderr, "[error] listmap_append_split_bin: split_key == NULL\n");
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
				listmap_append_str(l, string);
			}
			else{
				listmap_append_bin(l, word, word_i-split_key_length);
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
		listmap_append_str(l, string);
	}
	else{
		listmap_append_bin(l, word, word_i);
	}
	str_reset(string);
	free(word);
	free(string);
	word = NULL;
	string = NULL;
}

void listmap_append_split_str(listmap *l, str *string, char *split_key){
	listmap_append_split_bin(l, string->value, string->length, split_key, 0);
}

void listmap_append_split_str_skip_space(listmap *l, str *string, char *split_key){
	listmap_append_split_bin(l, string->value, string->length, split_key, 1);
}

void listmap_append_split_char(listmap *l, char *value, char *split_key){
	listmap_append_split_bin(l, value, strlen(value), split_key, 0);
}

void listmap_append_split_char_skip_space(listmap *l, char *value, char *split_key){
	listmap_append_split_bin(l, value, strlen(value), split_key, 1);
}

listmap new_listmap_from_split_bin(char *value, int value_length,
	char *split_key, byte skip_space){
	listmap l = new_listmap();
	listmap_append_split_bin(&l, value, value_length, split_key, skip_space);
	return l;
}

listmap *new_listmap_p_from_split_bin(char *value, int value_length,
	char *split_key, byte skip_space){
	listmap *l = new_listmap_p();
	listmap_append_split_bin(l, value, value_length, split_key, skip_space);
	return l;
}

listmap new_listmap_from_split_str(str *string, char *split_key){
	listmap l = new_listmap();
	listmap_append_split_bin(&l, string->value, string->length, split_key, 0);
	return l;
}

listmap *new_listmap_p_from_split_str(str *string, char *split_key){
	listmap *l = new_listmap_p();
	listmap_append_split_bin(l, string->value, string->length, split_key, 0);
	return l;
}

listmap new_listmap_from_split_str_skip_space(str *string, char *split_key){
	listmap l = new_listmap();
	listmap_append_split_bin(&l, string->value, string->length, split_key, 1);
	return l;
}

listmap *new_listmap_p_from_split_str_skip_space(str *string, char *split_key){
	listmap *l = new_listmap_p();
	listmap_append_split_bin(l, string->value, string->length, split_key, 1);
	return l;
}

listmap new_listmap_from_split_char(char *value, char *split_key){
	listmap l = new_listmap();
	listmap_append_split_bin(&l, value, strlen(value), split_key, 0);
	return l;
}

listmap *new_listmap_p_from_split_char(char *value, char *split_key){
	listmap *l = new_listmap_p();
	listmap_append_split_bin(l, value, strlen(value), split_key, 0);
	return l;
}

listmap new_listmap_from_split_char_skip_space(char *value, char *split_key){
	listmap l = new_listmap();
	listmap_append_split_bin(&l, value, strlen(value), split_key, 1);
	return l;
}

listmap *new_listmap_p_from_split_char_skip_space(char *value, char *split_key){
	listmap *l = new_listmap_p();
	listmap_append_split_bin(l, value, strlen(value), split_key, 1);
	return l;
}

void listmap_append_split_bin_space(listmap *l, char *value, int value_length){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr,
			"[error] listmap_append_split_bin_space: value == NULL\n");
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
				listmap_append_bin(l, word, word_i);
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
		listmap_append_bin(l, word, word_i);
	}
	free(word);
	word = NULL;
}

listmap new_listmap_from_split_bin_space(char *value, int value_length){
	listmap l = new_listmap();
	listmap_append_split_bin_space(&l, value, value_length);
	return l;
}

listmap *new_listmap_p_from_split_bin_space(char *value, int value_length){
	listmap *l = new_listmap_p();
	listmap_append_split_bin_space(l, value, value_length);
	return l;
}

listmap new_listmap_from_split_str_space(str *string){
	listmap l = new_listmap();
	listmap_append_split_bin_space(&l, string->value, string->length);
	return l;
}

listmap *new_listmap_p_from_split_str_space(str *string){
	listmap *l = new_listmap_p();
	listmap_append_split_bin_space(l, string->value, string->length);
	return l;
}

listmap new_listmap_from_split_char_space(char *value){
	listmap l = new_listmap();
	listmap_append_split_bin_space(&l, value, strlen(value));
	return l;
}

listmap *new_listmap_p_from_split_char_space(char *value){
	listmap *l = new_listmap_p();
	listmap_append_split_bin_space(l, value, strlen(value));
	return l;
}
