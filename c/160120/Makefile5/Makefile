DIR:=./Debug/
EXE:=main.exe
OBJS:=main.o
LIBMULSO:=libmul.so
CFLAGS:= -fpic
CC:=gcc
$(DIR)$(EXE):$(DIR)$(OBJS) $(DIR)$(LIBMULSO)
	$(CC) -o $@  $< -L$(DIR) -lmul
$(DIR)$(LIBMULSO):$(DIR)mul.o
	$(CC) -shared -o $@  $<
$(DIR)main.o:main.c
	$(CC) -c $^ -o $@
$(DIR)mul.o:mul.c
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY:rebuild clean
rebuild:clean $(DIR)$(EXE)
clean:
	rm -rf $(DIR)*.o $(DIR)*.exe $(DIR)*.so

