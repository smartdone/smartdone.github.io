#import <stdio.h>

__attribute__((constructor))
void hook_init() {
	printf("hook init\n");
}
