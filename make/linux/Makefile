.PHONY: all, build, clean

all: build clean

build:
	@cp ../../lepton/*.* ./
	@rm -f adhoc_sim complex.o sparse_matrix.o measurement.o
	@gcc -c complex.c
	@gcc -c sparse_matrix.c
	@gcc -c measurement.c
	@gcc adhoc_sim.c complex.o sparse_matrix.o measurement.o -o adhoc_sim -lm
	@echo "Processing ... !"

clean:
	@rm -f *.c *.h *.o
