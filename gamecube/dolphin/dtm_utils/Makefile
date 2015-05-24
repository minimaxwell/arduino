CC=gcc
CFLAGS=-Wall -Wextra -Iinclude 


LIB_DTM=dtm

all : lib/lib$(LIB_DTM).a

lib/lib%.a : bin/%.o
	ar rcs $@ $^

bin/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean : 
	rm -f bin/*.o
	rm -f lib/*.a
