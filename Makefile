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

all: $(BIN_DIR)/t2fs.o
	ar rcs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/t2fs.o $(LIB_DIR)/apidisk.o $(LIB_DIR)/bitmap2.o

$(BIN_DIR)/t2fs.o: $(SRC_DIR)/t2fs.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)/t2fs.o -I$(INC_DIR) $(SRC_DIR)/t2fs.c

# TESTE #
teste: $(TST_DIR)/display
	$(TST_DIR)/display

$(TST_DIR)/display: all $(TST_DIR)/display.c
	$(CC) $(CFLAGS) -o $(TST_DIR)/display $(TST_DIR)/display.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	rm -rf $(TST_DIR)/display
