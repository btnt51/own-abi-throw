#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

namespace __cxxabiv1
{
    struct __class_type_info{
        virtual void foo() {}
    } ti;
} // namespace __cxxabiv1


#define EXCEPTION_BUFF_SIZE 256
char exception_buff[EXCEPTION_BUFF_SIZE];

extern "C" {
    void* __cxa_allocate_exception(size_t thrown_size)
    {
        printf("alloc exception %i\n", (int)thrown_size);
        if(thrown_size > EXCEPTION_BUFF_SIZE) 
            printf("Exception to big!\n");
        return &exception_buff;
    }

    void __cxa_free_excpetion(void *thrown_exception);

#include <unwind.h>
    void __cxa_throw(void *thrown_exception, struct type_info *tinfo,void (*dest)(void*))
    {
        printf("throw\n");
        //__cxa_throw never returns
        exit(0);
    }
}//extern "C"