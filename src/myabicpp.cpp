#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace __cxxabiv1 {
    struct __class_type_info {
        virtual void foo() {}
    } ti;
}

#define EXCEPTION_BUFF_SIZE 255
char exception_buff[EXCEPTION_BUFF_SIZE];

extern "C" {

void* __cxa_allocate_exception(size_t thrown_size)
{
    printf("alloc ex %i\n", (int)(thrown_size));
    if (thrown_size > EXCEPTION_BUFF_SIZE) printf("Exception too big");
    return &exception_buff;
}

void __cxa_free_exception(void *thrown_exception);


#include <unwind.h>

typedef void (*unexpected_handler)(void);
typedef void (*terminate_handler)(void);

struct __cxa_exception { 
	std::type_info *	exceptionType;
	void (*exceptionDestructor) (void *); 
	unexpected_handler	unexpectedHandler;
	terminate_handler	terminateHandler;
	__cxa_exception *	nextException;

	int			handlerCount;
	int			handlerSwitchValue;
	const char *		actionRecord;
	const char *		languageSpecificData;
	void *			catchTemp;
	void *			adjustedPtr;

	_Unwind_Exception	unwindHeader;
};

void __cxa_throw(void* thrown_exception, struct type_info *tinfo, void (*dest)(void*))
{
    printf("__cxa_throw called\n");

    __cxa_exception *header = ((__cxa_exception *) thrown_exception - 1);
    _Unwind_RaiseException(&header->unwindHeader);

    // __cxa_throw never returns
    printf("no one handled __cxa_throw, terminate!\n");
    exit(0);
}


void __cxa_begin_catch()
{
    printf("begin FTW\n");
}

void __cxa_end_catch()
{
    printf("end FTW\n");
}

typedef const uint8_t * LSDA_ptr;

struct LSDA_Header{
    LSDA_Header(LSDA_ptr *lsda){
        LSDA_ptr read_ptr = *lsda;

        lsda_start_encoding = read_ptr[0];
        lsda_type_encoding = read_ptr[1];
        lsda_call_site_table_length = read_ptr[2];

        *lsda = read_ptr +sizeof(LSDA_Header);
    }


    uint8_t lsda_start_encoding;
    uint8_t lsda_type_encoding;
    uint8_t lsda_call_site_table_length;

};


struct LSDA_CS_Header
{
    LSDA_CS_Header(LSDA_ptr *lsda)
    {
        LSDA_ptr read_ptr = *lsda;
        encoding = read_ptr[0];
        length = read_ptr[1];
        *lsda = read_ptr + sizeof(LSDA_CS_Header);
    }


    uint8_t encoding;
    uint8_t length;
};


struct LSDA_CS
{
    LSDA_CS(LSDA_ptr *lsda)
    {
        LSDA_ptr read_ptr = *lsda;
        cs_start = read_ptr[0];
        cs_len = read_ptr[1];
        cs_lp = read_ptr[2];
        cs_action = read_ptr[3];
        *lsda = read_ptr + sizeof(LSDA_CS);
    }


    uint8_t cs_start;
    uint8_t cs_len;
    uint8_t cs_lp;
    uint8_t cs_action;
};



_Unwind_Reason_Code __gxx_personality_v0 (
                     int version, _Unwind_Action actions, uint64_t exceptionClass,
                     _Unwind_Exception* unwind_exception, _Unwind_Context* context)
{
    if (actions & _UA_SEARCH_PHASE)
    {
        printf("Personality function, lookup phase\n");
        return _URC_HANDLER_FOUND;
    } else if (actions & _UA_CLEANUP_PHASE) {
        printf("Personality function, cleanup\n");

        uintptr_t throw_ip = _Unwind_GetIP(context) - 1;

        LSDA_ptr lsda = (uint8_t*)_Unwind_GetLanguageSpecificData(context);
        LSDA_Header header(&lsda);
        LSDA_CS_Header cs_header(&lsda);

        const LSDA_ptr cs_table_end = lsda + cs_header.length;

        const uint8_t *lsda_cs_table_base = lsda + sizeof(LSDA_Header) + sizeof(LSDA_CS_Header);

        while(lsda < lsda_cs_table_base)
        {
            LSDA_CS cs(&lsda);

            if(not cs.cs_lp) continue;
        

            uintptr_t funcStart = _Unwind_GetRegionStart(context);

            uintptr_t try_start = funcStart + cs.cs_start;
            uintptr_t try_end = try_start + cs.cs_len;

            if(throw_ip < try_start) continue;
            if(throw_ip > try_end) continue;

            int r0 = __builtin_eh_return_data_regno(0);
            int r1 = __builtin_eh_return_data_regno(1);

            _Unwind_SetGR(context, r0, (uintptr_t)(unwind_exception));

            _Unwind_SetGR(context, r1, (uintptr_t)(1));
            _Unwind_SetIP(context, funcStart + cs.cs_lp);
            break;
        }
        return _URC_INSTALL_CONTEXT;
    } else {
        printf("Personality function, error\n");
        return _URC_FATAL_PHASE1_ERROR;
    }
}


}
