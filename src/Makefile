all: app

mkdir := mkdir build; mkdir build/obj
throw.o: throw.cpp throw.h
		@g++ -c -o build/obj/throw.o -O0 -ggdb throw.cpp

myabicpp.o: myabicpp.cpp
		@g++ -c -o build/obj/myabicpp.o -O0 -ggdb myabicpp.cpp

main.o: main.c
		${mkdir}; g++ -c -o build/obj/main.o -O0 -ggdb main.c

throw.s: throw.cpp
		@g++ -S throw.cpp

app: main.o throw.o myabicpp.o
		@gcc build/obj/main.o build/obj/throw.o build/obj/myabicpp.o -O0 -g -ggdb -o build/app
		@./build/app
		@rm -rf build
		
.PHONY: clean run

clean:
	rm -rf build

run: 
	./build/app