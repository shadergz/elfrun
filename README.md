# elfrun

A 64-bit binary loader for x86_64

The binaries may not have or need external libraries to function with 100% accuracy
No symbols are resolved at runtime - Only the binary is mapped into memory

### To build the test binary
- gcc -O1 -c main.c -o main.o
- ld -static -o main main.o
