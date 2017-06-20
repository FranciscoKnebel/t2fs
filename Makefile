#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
#

CC=gcc
CFLAGS=-Wall -g
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TST_DIR=./teste

all: $(LIB_DIR)/t2fs.o $(LIB_DIR)/disk.o $(LIB_DIR)/aux.o
	ar rcs $(LIB_DIR)/libt2fs.a $(LIB_DIR)/t2fs.o $(LIB_DIR)/apidisk.o $(LIB_DIR)/bitmap2.o $(LIB_DIR)/disk.o $(LIB_DIR)/aux.o

$(LIB_DIR)/t2fs.o: $(SRC_DIR)/t2fs.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/t2fs.o -I$(INC_DIR) $(SRC_DIR)/t2fs.c

$(LIB_DIR)/disk.o: $(SRC_DIR)/disk.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/disk.o -I$(INC_DIR) $(SRC_DIR)/disk.c

$(LIB_DIR)/aux.o: $(SRC_DIR)/aux.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/aux.o -I$(INC_DIR) $(SRC_DIR)/aux.c

# TESTE #
teste: $(BIN_DIR)/display $(BIN_DIR)/disk
	$(BIN_DIR)/disk

$(BIN_DIR)/display: all $(TST_DIR)/display.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/display $(TST_DIR)/display.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/disk: all $(TST_DIR)/disk.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/disk $(TST_DIR)/disk.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	rm -rf $(TST_DIR)/display $(TST_DIR)/disk
