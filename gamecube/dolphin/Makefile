CC=gcc
CFLAGS=-Wall -Wextra -Iinclude -Idtm_utils/include -Ldtm_utils/lib
LIB_DEP=dtm_utils/lib/libdtm.a

all : read_head

read_head : bin/read_head

$(LIB_DEP) : 
	make -C dtm_utils 

bin/% : bin/%.o $(LIB_DEP)
	$(CC) $(CFLAGS) -static $^ -ldtm -o $@

bin/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f bin/*
	make -C dtm_utils clean
