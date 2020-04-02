#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>

extern int errno ;

void* aligned_malloc(size_t required_bytes, size_t alignment)
{
    void* p1; // original block
    void** p2; // aligned block
    int offset = alignment - 1 + sizeof(void*);
    if ((p1 = (void*)malloc(required_bytes + offset)) == NULL)
    {
        return NULL;
    }
    p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void aligned_free(void *p)
{
    free(((void**)p)[-1]);
}




void run_pic_from_buffer(void* pic_buffer, size_t pic_size)
{
    int errnum;

    if ( 0 != mprotect(pic_buffer, pic_size, PROT_READ | PROT_EXEC))
    {
        errnum = errno;
        fprintf(stderr, "mprotect failed! (%d) : %s\n", errnum, strerror(errnum));
        return;
    }

    void (*entry_point)();
    entry_point = pic_buffer;
    entry_point();
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

    size_t page_size = sysconf(_SC_PAGE_SIZE);
    printf("page size : %d\n", page_size);
    size_t pic_size = buf.st_size + (page_size - (buf.st_size % page_size));
    printf("pic_size : %d\n", pic_size);

    void* file_contents = aligned_malloc(pic_size, page_size);
    if (NULL == file_contents)
    {
        fprintf(stderr, "malloc failed!\n");
        return;
    }
    printf("malloc addr : %p\n", file_contents);

    ssize_t bytes_read = read(fd, file_contents, buf.st_size);
    if (buf.st_size != bytes_read)
    {
        errnum = errno;
        fprintf(stderr, "read failed! (%d) : %s\n", errnum, strerror(errnum));
        return;
    }


    run_pic_from_buffer(file_contents, pic_size);

    aligned_free(file_contents);
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