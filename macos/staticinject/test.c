#import <stdio.h>
#import <string.h>

int crack(const char *str) {
	char *ss = "123456";
	if(strcmp(str, ss) == 0) {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[] ) {
	char in[10];
	printf("password:\n");
	scanf("%s", in);
	if(crack(in) == 1) {
		printf("success\n");
	}else{
		printf("failed\n");
	}
	return 0;
}

