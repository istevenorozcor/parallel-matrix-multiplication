GCC = gcc
MPICC = mpicc
oT = -fopenmp -O3

CFLAGS = -lm

oL= Otime.c

BINDIR = ../
PROGS = $(BINDIR)MM1c $(BINDIR)MM1r $(BINDIR)MPI_MM1c $(BINDIR)MPI_MM1r

all: MM1c MM1r MPI_MM1c MPI_MM1r

clean:
	$(RM) $(PROGS)

MM1c:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@ $(CFLAGS)

MM1r:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@ $(CFLAGS)

MPI_MM1c:
	$(MPICC) $(oT) $(oL) $@.c -o $(BINDIR)$@ $(CFLAGS)

MPI_MM1r:
	$(MPICC) $(oT) $(oL) $@.c -o $(BINDIR)$@ $(CFLAGS)
