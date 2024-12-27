.PHONY: all, build, clean

all: build clean

CC := ia16-elf-gcc
CCFLAGS := -mcmodel=medium -Os -s -mtune=i8088 -march=i8088
BIN := ../../bin/dos
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
SIMULATORS := ../../lepton/simulators
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/sim_bell.exe $(BIN)/drafter.exe $(BIN)/statevec.exe $(BIN)/*.o
	@$(CC) $(CCFLAGS) -c $(LIB)/complex.c -o $(BIN)/complex.o
	@$(CC) $(CCFLAGS) -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o
	@$(CC) $(CCFLAGS) -c $(LIB)/measurement.c -o $(BIN)/measurement.o
	@$(CC) $(CCFLAGS) -c $(LIB)/gate.c -o $(BIN)/gate.o
	@$(CC) $(CCFLAGS) $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/sim_bell.exe
	@$(CC) $(CCFLAGS) $(TOOLS)/drafter.c -o $(BIN)/drafter.exe
	@$(CC) $(CCFLAGS) $(SIMULATORS)/statevec.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o $(BIN)/gate.o -o $(BIN)/statevec.exe

clean:
	@echo "Cleaning ... !"
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o