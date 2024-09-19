.PHONY: all, build, clean

all: build clean

CC := zcc
CCFLAGS := +cpm
BIN := ../../bin/cpm
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
SIMULATORS := ../../lepton/simulators
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/*.com $(BIN)/*.o
	@$(CC) $(CCFLAGS) -c $(LIB)/complex.c -o $(BIN)/complex.o
	@$(CC) $(CCFLAGS) -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o
	@$(CC) $(CCFLAGS) -c $(LIB)/measurement.c -o $(BIN)/measurement.o
	@$(CC) $(CCFLAGS) -c $(LIB)/gate.c -o $(BIN)/gate.o
	@$(CC) $(CCFLAGS) $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/simbell.com -lmath48 -DAMALLOC -lndos
	@$(CC) $(CCFLAGS) $(TOOLS)/drafter.c -o $(BIN)/drafter.com -DAMALLOC
	@$(CC) $(CCFLAGS) $(SIMULATORS)/statevec.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/statevec.com -lmath48 -DAMALLOC

clean:
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o
