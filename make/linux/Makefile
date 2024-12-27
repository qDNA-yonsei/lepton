.PHONY: all, build, clean

all: build clean

CC := gcc
BIN := ../../bin/linux
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
SIMULATORS := ../../lepton/simulators
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/sim_bell $(BIN)/drafter $(BIN)/statevec $(BIN)/*.o
	@$(CC) -c $(LIB)/complex.c -o $(BIN)/complex.o
	@$(CC) -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o
	@$(CC) -c $(LIB)/measurement.c -o $(BIN)/measurement.o
	@$(CC) -c $(LIB)/gate.c -o $(BIN)/gate.o
	@$(CC) $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/sim_bell -lm
	@$(CC) $(TOOLS)/drafter.c -o $(BIN)/drafter
	@$(CC) $(SIMULATORS)/statevec.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/statevec -lm

clean:
	@echo "Cleaning ... !"
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o