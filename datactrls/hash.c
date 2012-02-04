long hash_bin(char *value, int value_length){
	//from python stringobject.c=>string_hash
	if (value_length == 0){
		return 0;
	}
	#if NULL_ARG_CHECK
	if (value == NULL){
		fprintf(stderr, "[error] char_find_from: value == NULL\n");
		return 0;
	}
	#endif
	int i;
	long hash = value[0] << 7;
	for (i=0; i<value_length; i++){
		hash = (1000003*hash) ^ value[i];
	}
	hash ^= value_length;
	if (hash == -1){
		hash = -2;
	}
	return hash;
}

long hash_char(char *value){
	return hash_bin(value, strlen(value));
}

long hash_str(str *string){
	return hash_bin(string->value, string->length);
}

long hash_int(int num){
	//from python intobject.c=>int_hash
	long hash = num;
	if (hash == -1){
		hash = -2;
	}
	return hash;
}
