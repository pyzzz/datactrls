void str_set_int(str *string_p, int num){
	byte minus = 0;
	int length = 1;
	char *value = NULL;
	int i;
	if (num < 0){
		minus = 1;
		num = 0 - num;
	}
	while (num >= int_pow(10, length)){
		length += 1;
	}
	if (minus == 1){
		length += 1;
	}
	value = (char *) malloc(sizeof(char)*(length+1));
	//sprintf ...
	for (i=1; i<=length; i++){
		value[length-i] = '0' + num % int_pow(10, i) / int_pow(10, i-1);
	}
	if (minus == 1){
		value[0] = '-';
	}
	value[length] = '\x00';
	str_set(string_p, value);
	free(value);
	value = NULL;
}

str new_str_from_int(int num){
	str string = new_str(NULL);
	str_set_int(&string, num);
	return string;
}

str *new_str_p_from_int(int num){
	str *string = new_str_p(NULL);
	str_set_int(string, num);
	return string;
}

int new_int_from_bin(char *value, int value_length){
	int num = 0;
	int i = 0;
	byte minus;
	char j;
	if (value_length == 0){
		return 0;
	}
	if (value[0] == '-'){
		minus = 1;
	}
	for (i=0+(minus?1:0); i<value_length; i++){
		j = value[i];
		if (j >= '0' && j <= '9')
		{
			num += (j - '0') * int_pow(10, (value_length-i-1));
		}
		else
		{
			fprintf(stderr, "[error] new_int_from_bin: [%s] not digit\n", value);
			return -1;
		}
	}
	if (minus == 1){
		num = 0 - num;
	}
	return num;
}

int new_int_from_char(char *value){
	int value_length = char_len(value);
	return new_int_from_bin(value, value_length);
}

int new_int_from_str(str *string_p){
	return new_int_from_bin(string_p->value, string_p->length);
}
