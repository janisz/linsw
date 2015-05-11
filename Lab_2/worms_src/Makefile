OBJS := helper.o main.o worms.o 
worms: $(OBJS)
	$(CC) -o worms $(OBJS) -l ncurses
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

