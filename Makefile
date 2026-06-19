CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?= -lm
BIN_DIR := bin

.PHONY: all clean directories limpieza limpieza2 subgraf xarxes5 suscep fases6

all: limpieza limpieza2 subgraf xarxes5 suscep fases6

directories:
	mkdir -p $(BIN_DIR)

limpieza: directories
	$(CC) $(CFLAGS) src/cleaning/limpieza.c -o $(BIN_DIR)/limpieza $(LDFLAGS)

limpieza2: directories
	$(CC) $(CFLAGS) src/cleaning/limpieza2.c -o $(BIN_DIR)/limpieza2 $(LDFLAGS)

subgraf: directories
	$(CC) $(CFLAGS) src/graph_sampling/subgraf.c -o $(BIN_DIR)/subgraf $(LDFLAGS)

xarxes5: directories
	$(CC) $(CFLAGS) src/simulation/Xarxes5.c -o $(BIN_DIR)/Xarxes5 $(LDFLAGS)

suscep: directories
	$(CC) $(CFLAGS) src/simulation/suscep.c -o $(BIN_DIR)/suscep $(LDFLAGS)

fases6: directories
	$(CC) $(CFLAGS) src/simulation/fases6.c -o $(BIN_DIR)/fases6 $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)
