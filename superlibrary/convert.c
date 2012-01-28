void str_set_int(str *string, int num){
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
	str_set_bin(string, value, length); //duplicate copy
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
	if (value == NULL){
		fprintf(stderr, "[error] new_int_from_bin: value == NULL\n");
		return -1;
	}
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
			fprintf(stderr, "[error] new_int_from_bin: not digit [\"%s\"]\n",
				value);
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

int new_int_from_str(str *string){
	return new_int_from_bin(string->value, string->length);
}

void hex_set_bin(str *hex, char *value, int value_length){
	int i;
	char j;
	char k;
	if (value == NULL){
		fprintf(stderr, "[error] hex_set_bin: value == NULL\n");
		return;
	}
	int new_length = value_length * 2;
	char *new_value = (char*) malloc(sizeof(char)*(new_length+1));
	for (i=0; i<value_length; i++){
		j = ((unsigned char) value[i])%16;
		k = ((unsigned char) value[i])/16;
		j = (j >= 10) ? ('a' + (j - 10)) : ('0' + j);
		k = (k >= 10) ? ('a' + (k - 10)) : ('0' + k);
		new_value[i*2+1] = j;
		new_value[i*2] = k;
	}
	new_value[new_length] = '\x00';
	str_reset(hex);
	hex->length = new_length;
	hex->value = new_value;
}

void hex_set_char(str *hex, char *value){
	int value_length = char_len(value);
	hex_set_bin(hex, value, value_length);
}

void hex_set(str* hex, str* string){
	hex_set_bin(hex, string->value, string->length);
}

str new_hex_from_bin(char *value, int value_length){
	str hex = new_str(NULL);
	hex_set_bin(&hex, value, value_length);
	return hex;
}

str *new_hex_p_from_bin(char *value, int value_length){
	str *hex = new_str_p(NULL);
	hex_set_bin(hex, value, value_length);
	return hex;
}

str new_hex_from_char(char *value){
	str hex = new_str(NULL);
	hex_set_char(&hex, value);
	return hex;
}

str *new_hex_p_from_char(char *value){
	str *hex = new_str_p(NULL);
	hex_set_char(hex, value);
	return hex;
}

str new_hex_from_str(str *string){
	str hex = new_str(NULL);
	hex_set(&hex, string);
	return hex;
}

str *new_hex_p_from_str(str *string){
	str *hex = new_str_p(NULL);
	hex_set(hex, string);
	return hex;
}

void str_set_hex_bin(str *string, char *hex_value, int hex_value_length){
	if (hex_value == NULL){
		fprintf(stderr, "[error] str_set_hex_bin: hex_value == NULL\n");
		return;
	}
	if (hex_value_length % 2 > 0){
		fprintf(stderr,
		"[error] str_set_hex_bin: hex_value_length %% 2 > 0 [\"%s\"]\n",
		hex_value);
		return;
	}
	int i;
	char j;
	char k;
	int new_length = hex_value_length / 2;
	char *new_value = (char*) malloc(sizeof(char)*(new_length+1));
	for (i=0; i<new_length; i++){
		j = hex_value[i*2];
		k = hex_value[i*2+1];
		//printf("j: %d, k: %d\n", j, k);
		new_value[i] = 0;
		if (j >= '0' && j <= '9'){
			new_value[i] += (j-'0')*16;
		}
		else if (j >= 'a' && j <= 'f'){
			new_value[i] += (j-'a'+10)*16;
		}
		else if (j >= 'A' && j <= 'F'){
			new_value[i] += (j-'A'+10)*16;
		}
		else{
			fprintf(stderr, "[error] str_set_hex_bin: not hex [\"%s\"]\n",
				hex_value);
			return;
		}
		if (k >= '0' && k <= '9'){
			new_value[i] += (k-'0');
		}
		else if (k >= 'a' && k <= 'f'){
			new_value[i] += (k-'a'+10);
		}
		else if (k >= 'A' && k <= 'F'){
			new_value[i] += (k-'A'+10);
		}
		else{
			fprintf(stderr, "[error] str_set_hex_bin: not hex [\"%s\"]\n",
				hex_value);
			return;
		}
	}
	new_value[new_length] = '\x00';
	str_reset(string);
	string->length = new_length;
	string->value = new_value;
}

void str_set_hex_char(str *string, char *hex_value){
	int hex_value_length = char_len(hex_value);
	str_set_hex_bin(string, hex_value, hex_value_length);
}

void str_set_hex(str *string, str *hex){
	str_set_hex_bin(string, hex->value, hex->length);
}

str new_str_from_hex_bin(char *hex_value, int hex_value_length){
	str string = new_str(NULL);
	str_set_hex_bin(&string, hex_value, hex_value_length);
	return string;
}

str *new_str_p_from_hex_bin(char *hex_value, int hex_value_length){
	str *string = new_str_p(NULL);
	str_set_hex_bin(string, hex_value, hex_value_length);
	return string;
}

str new_str_from_hex_char(char *hex_value){
	str string = new_str(NULL);
	int hex_value_length = char_len(hex_value);
	str_set_hex_bin(&string, hex_value, hex_value_length);
	return string;
}

str *new_str_p_from_hex_char(char *hex_value){
	str *string = new_str_p(NULL);
	int hex_value_length = char_len(hex_value);
	str_set_hex_bin(string, hex_value, hex_value_length);
	return string;
}

str new_str_from_hex(str *hex){
	str string = new_str(NULL);
	str_set_hex_bin(&string, hex->value, hex->length);
	return string;
}

str *new_str_p_from_hex(str *hex){
	str *string = new_str_p(NULL);
	str_set_hex_bin(string, hex->value, hex->length);
	return string;
}