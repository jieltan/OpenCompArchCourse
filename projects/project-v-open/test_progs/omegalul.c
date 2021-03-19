extern void crt;
long haha(long a) {
	return ~a;
}

int main() {
	int a = 29038;
	int b = a + 12;
	int* c;
	*c = b;
	a = haha(a);
	
	
	return 0;
}
