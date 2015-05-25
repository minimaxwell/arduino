CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude -Idtm_utils/include -Ldtm_utils/lib
LIB_DEP=dtm_utils/lib/libdtm.a

all : read_head test_duplicate test_create_from_scratch gc_to_dolphin

read_head : bin/read_head

test_duplicate : bin/test_duplicate

test_create_from_scratch : bin/test_create_from_scratch

gc_to_dolphin : bin/gc_to_dolphin

$(LIB_DEP) : 
	make -C dtm_utils 

bin/% : bin/%.o $(LIB_DEP)
	$(CC) $(CFLAGS) -static $^ -ldtm -o $@

bin/%.o : src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f bin/*
	make -C dtm_utils clean
