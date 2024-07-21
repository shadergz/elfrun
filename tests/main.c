extern void exit(int code);

__asm(".globl _start;");

__asm(".intel_syntax;"
	"exit:;"
	"mov %eax, 0x3c;"
	"syscall;"
	".att_syntax;"
);

__asm(".intel_syntax;"
	"_start:;"
	"xor %eax, %eax;"
	"mov %ebx, %eax;"
	"mov %ecx, %eax;"
	"call main;"
	"jmp exit;"
	".att_syntax;"
);

int fibo(int x) {
	if (x <= 1)
		return x;
	return fibo(x - 1) + fibo(x - 2); 
}

static const char* format = "Fibo of value X is Y\n";

void int2str(char** buffer, int integer) {
	if (integer < 0) {
		*(*buffer)++ = '-';
		integer &= 0 << 31;
	}
	if (integer == 0) {
		*(*buffer)++ = '0';
		return;
	}
	int inv = 0;
	while (integer > 0) {
		int value = integer % 10;
		integer /= 10;
		(*buffer)[inv++] = value + '0';
	}
	for (int add = inv - 1; inv && add > 0; add--) {
		int swap = (*buffer)[add];
		(*buffer)[add] = (*buffer)[add - 1];
		(*buffer)[add - 1] = (char)swap;
	}
	*buffer += inv;
}

void print(int x, int y) {
	char buffer[24];
	char *ptr = buffer;
	for (int fmt = 0; format[fmt] != '\0'; fmt++) {
		switch (format[fmt]) {
			case 'X':
				int2str(&ptr, x);
				fmt++;
				break;
			case 'Y':
				int2str(&ptr, y);
				fmt++;
				break;
		}
		*ptr++ = format[fmt];
	}
	*ptr = '\0';

	ptr = buffer;
	int len;
	for (len = 0; *ptr != '\0'; len++)
		ptr++;
	ptr = buffer;

	__asm(
		"mov %1, %%edx;"
		"mov %0, %%rsi;"
		"mov $0, %%edi;"
		"mov $1, %%eax;"
		"syscall;"
		:: "r" (ptr), "r" (len)
	);
}

static int queries[] = {1, 2, 5, 6, 3, 8, 9, 12, 11};
int main() {
	for (int qt = 0; qt < sizeof(queries) / sizeof(int); qt++) {
		int ask = queries[qt];
		int response = fibo(ask);
		print(ask, response);
	}
	return 0;
}
