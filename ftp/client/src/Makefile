
ELF:=main
OBJS:=main.c input.c command.c

all:$(ELF)
$(ELF):$(OBJS)
	gcc $^ -o $@ -g -I ../include/
.PHONY:clean

clean:
	rm -f $(ELF) 

