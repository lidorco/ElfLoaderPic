#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

extern int errno ;


void run_pic_from_buffer(void* pic_buffer)
{

}


void run_pic_from_file(char* file_name)
{
    int errnum;
    struct stat buf;

    int fd = open(file_name, O_RDONLY);


    if( 0 != stat(file_name, &buf))
    {
        errnum = errno;
        fprintf(stderr, "stat failed! (%d) : %s\n", errnum, strerror(errnum));
        return;
    }

    printf("file size : %ld\n", buf.st_size);

    void* file_contents = malloc(buf.st_size);
    if (NULL == file_contents)
    {
        fprintf(stderr, "malloc failed!\n");
        return;
    }

    ssize_t bytes_read = read(fd, file_contents, buf.st_size);
    if (buf.st_size != bytes_read)
    {
        errnum = errno;
        fprintf(stderr, "read failed! (%d) : %s\n", errnum, strerror(errnum));
        return;
    }
    printf("%s", (char*)file_contents);

    run_pic_from_buffer(file_contents);

    free(file_contents);
}

int main(int argc, char** argv)
{
    if (2 != argc)
    {
        printf("Missing pic code file argument!\n");
        return 1;
    }

    run_pic_from_file(argv[1]);

    return 0;
}