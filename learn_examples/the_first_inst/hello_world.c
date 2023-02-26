#include <stdio.h>

__attribute((constructor)) void hello () {
	printf("Hello World\n");
}

__attribute((destructor)) void goodbye () {
	printf("Goodbye, Cruel OS World\n");
}

int main() {
}



