#define DATA_TYPE_NULL 0
#define DATA_TYPE_INT 10
#define DATA_TYPE_STR 20

struct list_data{
	int num;
	str *string;
	byte type;
};
typedef struct list_data list_data;

list_data *new_list_data_p(){
	list_data *data = malloc(sizeof(list_data));
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

void list_data_set_bin(list_data *data, char *value, int value_length){
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] list_data_set_bin: value == NULL\n");
		return;
	}
	#endif
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
