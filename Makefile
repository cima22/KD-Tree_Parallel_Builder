# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

#tool macros
CC := gcc
CC_MPI := mpicc
OMP_FLAGS := -fopenmp -std=gnu99
MPI_FLAGS := -std=gnu99
LDLIBS := -lm
DBG_FLAGS := -g

.PHONY: clean serial debug

all: kdtree_omp kdtree_mpi

kdtree_omp: kdtree_omp.o
	${CC} ${OMP_FLAGS} -o ${BIN_PATH}/kdtree_omp ${OBJ_PATH}/kdtree_omp.o

kdtree_mpi: kdtree_mpi.o
	${CC_MPI} -o ${BIN_PATH}/kdtree_mpi ${OBJ_PATH}/kdtree_mpi.o ${LDLIBS}

kdtree_omp.o: ${SRC_PATH}/kdtree_omp.c
	${CC} ${OMP_FLAGS} -o ${OBJ_PATH}/kdtree_omp.o -c ${SRC_PATH}/kdtree_omp.c

kdtree_mpi.o: ${SRC_PATH}/kdtree_mpi.c
	${CC_MPI} ${MPI_FLAGS} -o ${OBJ_PATH}/kdtree_mpi.o -c ${SRC_PATH}/kdtree_mpi.c

serial: kdtree_serial.o
	${CC} ${OMP_FLAGS} -o ${BIN_PATH}/kdtree_serial ${OBJ_PATH}/kdtree_serial.o

kdtree_serial.o: ${SRC_PATH}/kdtree.c
	${CC} -o ${OBJ_PATH}/kdtree_serial.o -c ${SRC_PATH}/kdtree.c

debug:
	${CC} ${DBG_FLAGS} ${OMP_FLAGS} -o ${DBG_PATH}/kdtree_omp ${SRC_PATH}/kdtree_omp.c
	${CC_MPI} ${DBG_FLAGS} ${MPI_FLAGS} -o ${DBG_PATH}/kdtree_mpi ${SRC_PATH}/kdtree_mpi.c ${LDLIBS}

view:
	${CC} ${OMP_FLAGS} -D DEBUG -o ${OBJ_PATH}/kdtree_omp.o -c ${SRC_PATH}/kdtree_omp.c
	${CC} ${OMP_FLAGS} -o ${BIN_PATH}/kdtree_omp ${OBJ_PATH}/kdtree_omp.o
	${CC_MPI} ${MPI_FLAGS} -D DEBUG -o ${OBJ_PATH}/kdtree_mpi.o -c ${SRC_PATH}/kdtree_mpi.c
	${CC_MPI} -o ${BIN_PATH}/kdtree_mpi ${OBJ_PATH}/kdtree_mpi.o ${LDLIBS}

clean:
	@echo "Removing all the objects and binaries..."
	rm -f ${BIN_PATH}/kdtree_omp ${BIN_PATH}/kdtree_mpi ${BIN_PATH}/kdtree_serial
	rm -f ${OBJ_PATH}/*.o
	rm -f ${DBG_PATH}/kdtree_omp ${DBG_PATH}/kdtree_mpi
	@echo "Done!"
