CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall
LDFLAGS=-lgpiod
OBJS := main.o boardctl.o time_operations.o morse.o morse_search_tree.o
morse: $(OBJS)
	$(CC) -o morse $(CFLAGS) $(OBJS) $(LDFLAGS)
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
