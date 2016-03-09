ELF:=../bin/client
OBJS:=client.c recv.c
$(ELF):$(OBJS)
	gcc $^ -o $@ -I ../include

.PHONY:clean
clean:
	rm -f $(ELF)
