

struct pic_globals {
    void (*log_function)(char*, unsigned int);
};

__attribute__ ((section(".pic_code")))
void logger(char* msg, unsigned int msg_length) {

    // push write parameters
    asm("movl %0, %%ecx": : "r"(msg) : );
    asm("movl %0, %%edx": : "r"(msg_length) : );

    asm("movl $0x4, %eax"); // syscall write
    asm("movl $0x1, %ebx"); // stdout is 1

    asm("int $0x80");
}


__attribute__ ((section(".pic_code")))
void asd_string(){

    asm(".ascii \"this is asm string\n\"");
    asm(".byte 0x00");
}

__attribute__ ((section(".pic_code")))
unsigned int strlen( const char *s )
{
    unsigned int n = 0;

    while ( s[n] ) ++n;

    return n;
}

__attribute__ ((section(".pic_code")))
void entry_point()
{
    struct pic_globals global;

    global.log_function = logger;

    global.log_function((char*)asd_string, strlen((char*)asd_string));
}


int main() {

    entry_point();
    return 0;
}




