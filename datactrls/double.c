double double_max(double x, double y){
	if (x > y){
		return x;
	}
	else{
		return y;
	}
}

double double_abs(double x){
	if (x < 0){
		return 0 - x;
	}
	return x;
}

double double_pow(double x, double y){
	double num = pow(x, y);
	return num;
}