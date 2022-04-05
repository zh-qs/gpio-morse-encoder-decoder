CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall
OBJS := boardctl.o time_operations.o morse.o main.o morse_search_tree.o
morse: $(OBJS)
	$(CC) -o morse $(CFLAGS) $(LDFLAGS) $(OBJS) -lgpiod
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
