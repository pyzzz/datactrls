int int_max(int x, int y){
	if (x > y){
		return x;
	}
	else{
		return y;
	}
}

int int_abs(int x){
	if (x < 0){
		return 0 - x;
	}
	return x;
}

int int_pow(int x, int y){
	/*int i;
	int num = x;
	for (i=0; i<(y-1); i++){
		num *= x;
	}*/
	int num = (int) pow(x, y);
	return num;
}