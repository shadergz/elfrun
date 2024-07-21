#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
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
	"xor %rbp, %rbp;"
	"xor %eax, %eax;"
	"mov %ebx, %ebx;"
	"mov %ecx, %ecx;"
	"pop %rdi;"
	"lea %rsi, [%rsp];"

	"call main;"
	"jmp exit;"
	".att_syntax;"
);

int fibo(int x) {
	if (x <= 1)
		return x;
	return fibo(x - 1) + fibo(x - 2); 
}

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

void print(const char* format, ...) {
	va_list va;
	va_start(va, format);
	char buffer[24];
	char *ptr = buffer;
	for (int fmt = 0; format[fmt] != '\0'; fmt++) {
        	bool isArg = false;
		if (format[fmt] == '%') {
			fmt++;
			isArg = true;
        	}
        	if (isArg) {
			switch (format[fmt]) {
			case 'd':
				int2str(&ptr, va_arg(va, int));
				fmt++;
				break;
			}
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
	va_end(va);
}

const char* no_argv = "No arguments were passed to the program\n";
const char* no_argc = "Argument count is zero\n";
static int queries[] = {1, 2, 5, 6, 3, 8, 9, 12, 11};

int main(int argc, char** argv) {
	if (argc == 0)
		print(no_argc);
	if (argv == NULL)
		print(no_argv);

	print("Printing the arguments: \n");
	while (*argv != NULL) {
		print(*argv++);
		print("\n");
	}

	for (int qt = 0; qt < sizeof(queries) / sizeof(int); qt++) {
		int ask = queries[qt];
		int response = fibo(ask);
		print("Fibo of value %d is %d\n", ask, response);
	}
	return 0;
}
