#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        puts("./test_a2A dev");
        return 1;
    }
    // open dev
    int fd = open(argv[1], O_RDWR);
    if(fd == -1) {
        puts("Fail to open devices!");
        return 1;
    }
    printf("Open devices success! Get fd=%d.\n", fd);
    // write
    puts("Now write...");
    char str[] = "i love linux, \nbut linux don't love me ~(#-#)~\n";
    printf("The string is:\n%s", str);
    if(write(fd, str, strlen(str)) == -1) {
        puts("Fail to write!");
        goto _exit;
    }
    // seek
    puts("Now turn back to start...");
    if(lseek(fd, 0, SEEK_SET) == -1) {
        puts("Fail to seek!");
        goto _exit;
    }
    // read
    puts("Now reading...");
    if(read(fd, str, strlen(str)) == -1) {
        puts("Fail to read!");
        goto _exit;
    }
    printf("The string is:\n%s", str);
    // release
_exit:
    puts("Now release dev...");
    close(fd);
	return 0;
}