main.exe:main.o mul.o
	gcc -o main.exe main.o mul.o
main.o:main.c
	gcc -c main.c
mul.o:mul.c
	gcc -c mul.c
.PHONY:clean
clean:
	rm -rf main.o mul.o
