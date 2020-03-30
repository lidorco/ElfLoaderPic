

struct pic_globals {
    void (*log_function)(char*, unsigned int);
};


void logger(char* msg, unsigned int msg_length) {

    // push write parameters
    asm("movl %0, %%ecx": : "r"(msg) : );
    asm("movl %0, %%edx": : "r"(msg_length) : );

    asm("movl $0x4, %eax"); // syscall write
    asm("movl $0x1, %ebx"); // stdout is 1

    asm("int $0x80");
}


int main(){

    struct pic_globals global;

    global.log_function = logger;

    global.log_function("asd\n", 4);

    return 0;
}




