#import "fishhook.h"
#import <unistd.h>
#import <stdio.h>

pid_t (*orig_getpid)() = NULL;

pid_t new_getpid() {
	return (*orig_getpid)() + 1;
}

int main(int argc, char *argv[]) {
	pid_t pid = getpid();
	printf("before hook pid = %d\n", pid);
    struct rebinding rebindings[1];
    rebindings[0].name = "getpid";
    rebindings[0].replacement = new_getpid;
    rebindings[0].replaced = (void*)&orig_getpid;
	rebind_symbols(rebindings, 1);
    pid = getpid();
    printf("after hook pid = %d\n", pid);
	return 0;
}
