ELF:=../bin/server
OBJS:=main.c  make_child.c  recv_fd.c  send_fd.c  send_n.c
CC:=gcc
$(ELF):$(OBJS)
	$(CC) $^ -o $@ -I ../include

.PHONY:clean
clean:
	rm -f $(ELF)

