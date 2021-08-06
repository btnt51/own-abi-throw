#include "throw.h"

extern  "C"{
    void sepuku(){
        throw Exception();
    }
}