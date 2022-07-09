#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>

void main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <shellcode>\n", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file: %s\n", strerror(errno));
        exit(1);
    }
    char *shellcode = (char *)malloc(sizeof(char) * 1024);
    int read_bytes = read(fd, shellcode, 1024);
    if (read_bytes == -1)
    {
        printf("Error reading file: %s\n", strerror(errno));
        exit(1);
    }
    close(fd);
    int shellcode_length = strlen(shellcode);
    char *buffer = (char *)malloc(sizeof(char) * (shellcode_length + 8));
    memcpy(buffer, shellcode, shellcode_length);
    int i;
    for (i = 0; i < shellcode_length; i++)
    {
        buffer[i + shellcode_length] = 0x90;
    }
    for (i = 0; i < shellcode_length; i++)
    {
        buffer[i] = 0xeb;
    }
    fd = open("ex_f.bin", O_WRONLY | O_CREAT, 0755);
    if (fd == -1)
    {
        printf("Error opening file: %s\n", strerror(errno));
        exit(1);
    }
    write(fd, buffer, shellcode_length + 8);
    close(fd);
    free(buffer);
    free(shellcode);
}
