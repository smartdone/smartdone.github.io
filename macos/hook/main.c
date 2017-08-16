#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

int main(int argc, const char *argv[]) {
    int fd = open(argv[0], O_RDONLY);
    uint32_t magic = 0;
    read(fd, (void*)&magic, 4);
    printf("Mach-O Magic Number: %x\n", magic);
    return 0;
}
