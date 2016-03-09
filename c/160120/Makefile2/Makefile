OBJS:=main.o mul.o
EXE:=main.exe
$(EXE):$(OBJS)
	gcc -o $(EXE) $(OBJS)
main.o:main.c
	gcc -c main.c
mul.o:mul.c
	gcc -c mul.c
.PHONY:clean
clean:
	rm -rf $(EXE) $(OBJS)
