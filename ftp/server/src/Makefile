ELF:=main.c command.c
OBJS:=main

all:$(OBJS)

$(OBJS):$(ELF)
	gcc $^ -o $@ -I ../include/ -g

.PHONY:clean

clean:
	rm -f $(OBJS)


