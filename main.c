#include <stdio.h>
#include <stdlib.h>

int main(){
		printf("Main running\n");
		system("dtruss -c ./program");
		system("strace -c ./program");
		printf("Main ending\n");
		return 0;
}

