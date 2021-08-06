#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

namespace __cxxabiv1 {
    struct __class_type_info {
        virtual void foo() {}
    } ti;
} // namespace __cxxabiv1


#define EXCEPTION_BUFF_SIZE 256
char exception_buff[EXCEPTION_BUFF_SIZE];

extern "C" {
    void* __cxa_allocate_exception(size_t thrown_size) {
        printf("alloc exception %i\n", (int)thrown_size);
        if(thrown_size > EXCEPTION_BUFF_SIZE) 
            printf("Exception to big!\n");
        return &exception_buff;
    }

    void __cxa_free_excpetion(void *thrown_exception);

#include <unwind.h>
    typedef void (*unexpected_handler)(void);
    typedef void (*terminate_handler)(void);

    struct __cxa_exception{
        std::type_info*    exceptionType;
        unexpected_handler unexpectedHandler;
        terminate_handler  terminateHandler;
        __cxa_exception*   nextException;
        void    (*DestructorType)(void *);

        int          handlerCount;
        int          handlerSwitchValue;
        const char * actionRecord;
        const char * LanguageSpecificData;
        void *       catchTemp;
        void *       adjustedPtr;

        _Unwind_Exception unwindHeader; 
    };


    void __cxa_throw(void *thrown_exception, struct type_info *tinfo,void (*dest)(void*)) {
        printf("__cxa_throw called\n");

        __cxa_exception *header = ((__cxa_exception *) thrown_exception -1);
        _Unwind_RaiseException(&header->unwindHeader);


        //__cxa_throw never returns
        printf("no one handled __cxa_throw, terminate!\n");
        exit(0);
    }

    void __cxa_begin_catch() {
        printf("begin FTW\n");
    }

    void __cxa_end_catch() {
        printf("end FTW\n");
    }

    _Unwind_Reason_Code __gxx_personality_v0(int version, 
                         _Unwind_Action actions, u_int64_t exceptionClass,
                        _Unwind_Exception* unwind_exception, _Unwind_Context* context) { 
        
        printf("Presonality function FTW\n");
    }
}//extern "C"