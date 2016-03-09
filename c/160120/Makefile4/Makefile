OBJC:=main.o mul.o
EXE:=main.exe
CC:=gcc
$(EXE):$(OBJC)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) -o $@ -c $^
.PHONY:clean
clean:
	rm -rf $(EXE) $(OBJC)

