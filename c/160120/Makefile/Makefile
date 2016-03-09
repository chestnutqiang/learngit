MAINC:=main.c
MULC:=mul.c
MAIN:=main.o
MUL:=mul.o
OBJC:=$(MAIN) $(MUL)
EXE:=main.exe
$(EXE):$(OBJC)
	gcc -o $@ $^
$(MAIN):$(MAINC)
	gcc -c $< -o $@
$(MUL):$(MULC)
	gcc -c $< -o $@
.PHONY:clean
clean:
	rm -rf $(EXE) $(OBJC)

