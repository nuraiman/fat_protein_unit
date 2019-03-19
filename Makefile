CC=g++
RELEASE = -O3
DEBUG = -O0 -g
RELWITHDBINFO = -g -O2
CFLAGS=--pedantic -Wall -fopenmp --std=c++17 -I.
DEPS = fat_protein_unit.cpp
OBJ = fat_protein_unit.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE) $(DEPS) -o fpu_simulator.run

debug: $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG) $(DEPS) -o fpu_sim.debug

.PHONY: clean

clean:
	rm -f *.o *~
