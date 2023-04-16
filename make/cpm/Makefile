.PHONY: all, build, clean

all: build clean

BIN := ../../bin/cpm
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
SIMULATORS := ../../lepton/simulators
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/*.com $(BIN)/*.o
	@zcc +cpm -c $(LIB)/complex.c -o $(BIN)/complex.o -lm
	@zcc +cpm -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o -lm
	@zcc +cpm -c $(LIB)/measurement.c -o $(BIN)/measurement.o -lm
	@zcc +cpm -c $(LIB)/gate.c -o $(BIN)/gate.o -lm
	@zcc +cpm $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o -o $(BIN)/sim_bell.com -lm -DAMALLOC -lndos
	@zcc +cpm -o $(BIN)/drafter.com $(TOOLS)/drafter.c
	@zcc +cpm $(SIMULATORS)/statevec.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/statevec.com -lm -DAMALLOC

clean:
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o