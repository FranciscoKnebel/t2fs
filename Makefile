CC=gcc
CFLAGS=-Wall -g
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TST_DIR=./teste

# GERAÇÂO DO T2FS #
LIB_OBJECTS=$(LIB_GENERATED_OBJECTS) $(LIB_DIR)/apidisk.o $(LIB_DIR)/bitmap2.o
LIB_GENERATED_OBJECTS=$(LIB_DIR)/t2fs.o $(LIB_DIR)/disk.o $(LIB_DIR)/parse.o $(HELPER_OBJECTS)

all: $(LIB_OBJECTS) $(LIB_DIR)/helpers
	ar rcs $(LIB_DIR)/libt2fs.a $(LIB_OBJECTS)

$(LIB_DIR)/t2fs.o: $(SRC_DIR)/t2fs.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/t2fs.o -I$(INC_DIR) $(SRC_DIR)/t2fs.c

$(LIB_DIR)/disk.o: $(SRC_DIR)/disk.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/disk.o -I$(INC_DIR) $(SRC_DIR)/disk.c

$(LIB_DIR)/parse.o: $(SRC_DIR)/parse.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/parse.o -I$(INC_DIR) $(SRC_DIR)/parse.c

## HELPERS ##
HELPER_OBJECTS=$(LIB_DIR)/helpers/print.o $(LIB_DIR)/helpers/util.o

$(LIB_DIR)/helpers: $(HELPER_OBJECTS)

$(LIB_DIR)/helpers/print.o: $(SRC_DIR)/helpers/print.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/print.o -I$(INC_DIR) $(SRC_DIR)/helpers/print.c

$(LIB_DIR)/helpers/util.o: $(SRC_DIR)/helpers/util.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/util.o -I$(INC_DIR) $(SRC_DIR)/helpers/util.c

# COMANDOS DE TESTE #

ifeq (teste,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

TEST_FILES=$(BIN_DIR)/display $(BIN_DIR)/disk $(BIN_DIR)/api $(BIN_DIR)/parse $(BIN_DIR)/util

# make teste _NOME_
teste: $(TEST_FILES)
	$(BIN_DIR)/$(RUN_ARGS)

$(BIN_DIR)/display: all $(TST_DIR)/display.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/display $(TST_DIR)/display.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/disk: all $(TST_DIR)/disk.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/disk $(TST_DIR)/disk.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/api: all $(TST_DIR)/api.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/api $(TST_DIR)/api.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/parse: all $(TST_DIR)/parse.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/parse $(TST_DIR)/parse.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/util: all $(TST_DIR)/util.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/util $(TST_DIR)/util.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)


# LIMPEZA #
clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	rm -rf $(LIB_GENERATED_OBJECTS)
	rm -rf $(TEST_FILES)
