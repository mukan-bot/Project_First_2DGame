#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

	char d[] = "‚ ";
	//for (int i = 0; i < strlen(d); i++) {
	//	char c = d[i];
	printf("0x%02X\n", d);
	//}


	system("pause");
}