OBJS := main.o
memo: $(OBJS)
	$(CC) -o memo $(OBJS)
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ -std=c99

