.PHONY: all, build, clean

all: build clean

BIN := ../../bin/linux
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
SIMULATORS := ../../lepton/simulators
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/sim_bell $(BIN)/drafter $(BIN)/statevec $(BIN)/*.o
	@gcc -c $(LIB)/complex.c -o $(BIN)/complex.o
	@gcc -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o
	@gcc -c $(LIB)/measurement.c -o $(BIN)/measurement.o
	@gcc -c $(LIB)/gate.c -o $(BIN)/gate.o
	@gcc $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/sim_bell -lm
	@gcc -o $(BIN)/drafter $(TOOLS)/drafter.c
	@gcc $(SIMULATORS)/statevec.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/statevec -lm

clean:
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o