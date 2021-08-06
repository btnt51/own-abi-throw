#include "throw.h"

struct Fake_Exception{};

void raise(){
    throw Exception();
}

void try_but_dont_catch(){
    try {
        raise();
    } catch (Fake_Exception&){
        printf("Running try_but_dont_catch::catch(Fake_Exception)\n");
    }

    printf("try_but_dont_catch handled an exception and resumed execution\n");
}

void catchit(){
    try {
        try_but_dont_catch;
    } catch(Exception&) {
        printf("Running try_but_dont_catch::catch(Exception&)\n");
    } catch(Fake_Exception&){
        printf("Running try_but_dont_catch::catch(Fake_Exception)\n");
    }

    printf("catchit handled an exception and resumed execution\n");
}

extern  "C"{
    void sepuku(){
        catchit();
    }
}