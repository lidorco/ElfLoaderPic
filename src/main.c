

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
void* get_eip()
{
    asm ("pop %eax;"
         "push %eax;"
         "ret");
}


__attribute__ ((section(".pic_code")))
char* asd_string(){
    unsigned int absolute_address_of_mov_instruction = 0;
    void *mov_address_pointer, *str_start_pointer;

mov_address:
    absolute_address_of_mov_instruction = get_eip();
    int a=0;
    if(++a>0) goto end;

str_start:
    asm(".ascii \"this is asd string\n\"");
    asm(".byte 0x00");

end:
    mov_address_pointer = &&mov_address;
    str_start_pointer = &&str_start;
    unsigned int bytes_until_str_start = str_start_pointer - mov_address_pointer;

    return absolute_address_of_mov_instruction - 5 + bytes_until_str_start; // minus 5 because call get_eip() is 5 bytes instruction
}

__attribute__ ((section(".pic_code")))
unsigned int strlen(const char *str)
{
    for (unsigned int i = 0; ; i++)
    {
        if (str[i] == '\0')
        {
            return i;
        }
    }
}


__attribute__ ((section(".pic_code")))
void* get_function_absolute_address(void* f)
{
    unsigned int absolute_address = 0;
    void* extra_bytes_start_pointer;
    void* function_call_pointer;
    void (*function)() = f;

extra_bytes_start:
    absolute_address = get_eip();
    int a=0;
    if(++a>0) goto end;

function_call:
    function();

end:
    extra_bytes_start_pointer = &&extra_bytes_start;
    function_call_pointer = &&function_call;

    unsigned int bytes_until_function_call = function_call_pointer - extra_bytes_start_pointer;

    signed int relative_address = (absolute_address - 5) + bytes_until_function_call + 1;

    signed int function_call_absolute_address = (absolute_address - 5) + bytes_until_function_call + 5;

    return (void*)(relative_address + function_call_absolute_address);
}


#define GET_FUNCTION_ABSOLUTE_ADDRESS(function, result) { \
                                                signed int function##_var_eip = (signed int)get_eip(); /* 4 bytes*/ \
                                                int a=0;/* 8 bytes*/ if(++a>0) goto function##_after_call; /*mov-8 add-5 cmp-5 jg-2*/\
                                                ((void (*)())(function))(); /*1 byte*/\
                                                function##_after_call: /*extra jmp and nop*/\
                                                result = (function##_var_eip+24+5) + *((signed int*)(function##_var_eip+25));\
                                                }

__attribute__ ((section(".pic_code.ENTRY_POINT")))
void entry_point()
{
    struct pic_globals global;

    signed int absolute_address;
    GET_FUNCTION_ABSOLUTE_ADDRESS(logger, absolute_address);

    global.log_function = (void (*)(char*, unsigned int))absolute_address ;


    global.log_function(asd_string(), strlen(asd_string()));
}


int main() {

    entry_point();
    return 0;
}




