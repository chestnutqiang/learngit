ELF:=main
OBJS:=main.c fd_que.c get_conf_value.c send_file.c thread_pool.c send_n.c
$(ELF):$(OBJS)
	gcc $^ -o $@ -I ../include -lpthread -g
.PHONY:clean

clean:
	rm -f $(ELF)
