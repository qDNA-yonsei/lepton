.PHONY: all, build, clean

all: build clean

build:
	@echo "Processing ... !"
	@cp ../../lepton/*.* ./
	@rm -f adhoc_sim drafter complex.o sparse_matrix.o measurement.o
	@gcc -c complex.c
	@gcc -c sparse_matrix.c
	@gcc -c measurement.c
	@gcc adhoc_sim.c complex.o sparse_matrix.o measurement.o -o adhoc_sim -lm
	@gcc drafter.c -o drafter
	
clean:
	@rm -f *.c *.h *.o
