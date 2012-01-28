struct file{
	FILE *fp;
	long size;
	byte mode_read;
	byte mode_write;
	byte mode_add;
	byte mode_bin;
};
typedef struct file file;

file new_file(){
	file io;
	io.fp = NULL;
	io.size = 0;
	io.mode_read = 0;
	io.mode_write = 0;
	io.mode_add = 0;
	io.mode_bin = 0;
	return io;
}

void file_reset(file *io){
	io->fp = NULL;
	io->size = 0;
	io->mode_read = 0;
	io->mode_write = 0;
	io->mode_add = 0;
	io->mode_bin = 0;
}

int is_file(char *path){
	//return bool
	FILE *fp = NULL;
	fp = fopen(path, "r");
	if (fp == NULL){
		return 0;
	}
	fclose(fp);
	fp = NULL;
	return 1;
}

int is_dir(char *path){
	//return bool
	DIR *dir = NULL;
	dir = opendir(path);
	if (dir == NULL){
		return 0;
	}
	closedir(dir);
	return 1;
}

long file_get_size(char *path){
	FILE *fp = NULL;
	fp = fopen(path, "r");
	if (fp == NULL){
		fprintf(stderr, "[error] file_get_size: fp == NULL\n");
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fclose(fp);
	fp = NULL;
	return size;
}

int file_remove(char *path){
	//return error
	int error = remove(path);
	if (error != 0){
		fprintf(stderr, "[error] file_remove: remove failed, error id [%d]\n",
			error);
	}
	return error;
}

int file_close(file *io){
	//return error
	if (io->fp == NULL){
		fprintf(stderr, "[error] file_close: io->fp == NULL\n");
		return -1;
	}
	int i = fclose(io->fp);
	file_reset(io);
	if (i != 0){
		fprintf(stderr, "[error] file_close: error id [%d]\n", i);
		return -2;
	}
	return 0;
}

int file_open(file *io, char *path, char *mode){
	//return error
	int i;
	if (mode == NULL){
		fprintf(stderr, "[error] file_open: mode == NULL\n");
		return -1;
	}
	if (io->fp != NULL){
		file_close(io);
	}
	file_reset(io);
	io->fp = fopen(path, mode);
	if (io->fp == NULL){
		if (mode[0] != 'r' && mode[0] != 'w' && mode[0] != 'a'){
			fprintf(stderr,
				"[error] file_open: mode not in (r, w, a) [%s, %s]\n",
				path, mode);
			return -2;
		}
		else{
			fprintf(stderr, "[error] file_open: file not exist [%s, %s]\n",
				path, mode);
			return -3;
		}
	}
	for (i=0; i<3; i++){
		if (mode[i] == '\x00'){
			break;
		}
		else if (mode[i] == 'r'){
			io->size = file_get_size(path);
			io->mode_read = 1;
		}
		else if (mode[i] == 'w'){
			io->mode_write = 1;
		}
		else if (mode[i] == 'a' || mode[i] == '+'){
			io->mode_add = 1;
		}
	}
	return 0;
}

int file_write_bin(file *io, char *value, int value_length){
	//return error
	if (io->fp == NULL){
		fprintf(stderr, "[error] file_write_bin: io->fp == NULL\n");
		return -1;
	}
	if (value == NULL){
		fprintf(stderr, "[error] file_write_bin: value == NULL\n");
		return -2;
	}
	if (io->mode_write != 1 && io->mode_add != 1){
		fprintf(stderr, "[error] file_write_bin: file not open for write\n");
		return -3;
	}
	int i;
	for (i=0; i<value_length; i++){
		if (fputc(value[i], io->fp) == EOF){
			fprintf(stderr, "[error] file_write_bin: write error\n");
			io->size += i;
			return -4;
		}
	}
	io->size += value_length;
	return 1;
}

int file_write_char(file *io, char *value){
	int value_length = char_len(value);
	return file_write_bin(io, value, value_length);
}

int file_write_str(file *io, str *string){
	return file_write_bin(io, string->value, string->length);
}

char *file_read_char(file *io, int size){
	int i;
	char j;
	if (io->fp == NULL){
		fprintf(stderr, "[error] file_read_char: io->fp == NULL\n");
		return NULL;
	}
	if (io->mode_read != 1){
		fprintf(stderr, "[error] file_read_char: file not open for read\n");
		return NULL;
	}
	char *value = (char*) malloc(sizeof(char)*(size+1));
	for (i=0; i<size; i++){
		j = fgetc(io->fp);
		if (j == EOF){
			fprintf(stderr, "[error] file_read_char: read error\n");
			value[i] = '\x00';
		}
		value[i] = j;
	}
	return value;
}

void file_read_str(file *io, str *string, int size){
	int i;
	if (io->fp == NULL){
		fprintf(stderr, "[error] file_read_char: io->fp == NULL\n");
		return;
	}
	if (io->mode_read != 1){
		fprintf(stderr, "[error] file_read_char: file not open for read\n");
		return;
	}
	int size_max = (int) (io->size - ftell(io->fp));
	if (size == -1 || size > size_max){
		size = size_max;
	}
	//printf("%ld %ld %d\n", io->size, ftell(io->fp), size_max);
	int length = -1;
	char *value = (char*) malloc(sizeof(char)*(size+1));
	for (i=0; i<size; i++){
		value[i] = fgetc(io->fp);
	}
	if (length == -1){
		length = size;
	}
	str_set_bin(string, value, length);
	free(value);
	value = NULL;
}
