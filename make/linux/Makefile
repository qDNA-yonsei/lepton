.PHONY: all, build, clean

all: build clean

BIN := ../../bin/linux
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/sim_bell $(BIN)/drafter $(BIN)/*.o
	@gcc -c $(LIB)/complex.c -o $(BIN)/complex.o
	@gcc -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o
	@gcc -c $(LIB)/measurement.c -o $(BIN)/measurement.o
	@gcc $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o -o $(BIN)/sim_bell -lm
	@gcc -o $(BIN)/drafter $(TOOLS)/drafter.c

clean:
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o