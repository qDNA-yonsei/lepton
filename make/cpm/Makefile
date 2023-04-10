.PHONY: all, build, clean

all: build clean

BIN := ../../bin/cpm
LIB := ../../lepton/lib
EXAMPLES := ../../lepton/examples
TOOLS := ../../lepton/tools

build:
	@echo "Processing ... !"
	@mkdir -p $(BIN)
	@rm -f $(BIN)/*.com $(BIN)/*.o
	@zcc +cpm -c $(LIB)/complex.c -o $(BIN)/complex.o --math-mbf32
	@zcc +cpm -c $(LIB)/sparse_matrix.c -o $(BIN)/sparse_matrix.o --math-mbf32
	@zcc +cpm -c $(LIB)/measurement.c -o $(BIN)/measurement.o --math-mbf32
	@zcc +msx -subtype=msxdos $(EXAMPLES)/sim_bell.c $(BIN)/complex.o $(BIN)/sparse_matrix.o $(BIN)/measurement.o -o $(BIN)/sim_bell.com --math-mbf32 -DAMALLOC -lndos
	@zcc +cpm -o $(BIN)/drafter.com $(TOOLS)/drafter.c

clean:
	@rm -f $(BIN)/*.c $(BIN)/*.h $(BIN)/*.o
