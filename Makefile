# You have to make two empty folders (objs and bin)
# We can push them ....... and delete this (TODO)

CC = g++ -pthread
CFLAGS = -I -w -Wall -std=c++20 -O3
OBJ_DIR = objs
SRC_DIR = src
BIN_DIR = bin

# Folders ans main of each solver of the QAP

QAP_DIR = $(SRC_DIR)/QAP
QAP = $(wildcard $(QAP_DIR)/*.cpp)

ES_DIR = $(SRC_DIR)/exact_solver
ES = $(wildcard $(ES_DIR)/solver/*.cpp)

LSS_DIR = $(SRC_DIR)/local_search_solver
LSS = $(wildcard $(LSS_DIR)/solver/*.cpp)

ILSS_DIR = $(SRC_DIR)/iterative_local_search_solver
ILSS = $(wildcard $(ILSS_DIR)/solver/*.cpp)

GAS_DIR = $(SRC_DIR)/genetic_algorithm_solver
GAS = $(wildcard $(GAS_DIR)/solver/*.cpp)

TS_DIR = $(SRC_DIR)/tabu_search_solver
TS = $(wildcard $(TS_DIR)/solver/*.cpp)

UTILS_DIR = $(SRC_DIR)/utils
CL = $(wildcard $(UTILS_DIR)/circular_list/*.cpp)

MAS_DIR = $(SRC_DIR)/memetic_solver
MAS = $(wildcard $(MAS_DIR)/solver/*.cpp)

THREAD_KILLER_DIR = $(SRC_DIR)/thread_killer
THREAD_KILLER = $(wildcard $(THREAD_KILLER_DIR)/*.cpp)

all: main_exact_solver main_local_search main_iterative_local_search main_benchmark

# QAP 

$(OBJ_DIR)/qap.o: $(QAP)
	$(CC) $(CFLAGS) -c $< -o $@

# Benchmark 

main_benchmark: $(OBJ_DIR)/qap.o $(OBJ_DIR)/es.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/ilss.o $(OBJ_DIR)/cl.o $(OBJ_DIR)/ts.o $(OBJ_DIR)/main_benchmark.o $(OBJ_DIR)/thKiller.o $(OBJ_DIR)/gas_mut.o $(OBJ_DIR)/gas_cros.o $(OBJ_DIR)/gas.o $(OBJ_DIR)/mas.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/main_benchmark.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Thread killer 

$(OBJ_DIR)/thKiller.o: $(THREAD_KILLER)
	$(CC) $(CFLAGS) -c $< -o $@

# Exact solver

main_exact_solver: $(OBJ_DIR)/qap.o $(OBJ_DIR)/es.o $(OBJ_DIR)/main_es.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/es.o: $(ES) $(QAP)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_es.o: $(ES_DIR)/main.cpp $(ES)
	$(CC) $(CFLAGS) -c $< -o $@

# Local search

main_local_search: $(OBJ_DIR)/qap.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/main_lss.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/lss.o: $(LSS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_lss.o: $(LSS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Iterative local search

main_iterative_local_search: $(OBJ_DIR)/qap.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/ilss.o $(OBJ_DIR)/main_ilss.o 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/ilss.o: $(ILSS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_ilss.o: $(ILSS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Genetic algorithm 

main_genetic_algorithm: $(OBJ_DIR)/qap.o $(OBJ_DIR)/gas_mut.o $(OBJ_DIR)/gas_cros.o $(OBJ_DIR)/gas.o $(OBJ_DIR)/main_gas.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/gas.o: $(GAS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/gas_mut.o: $(GAS_DIR)/solver/mutation/mutation.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/gas_cros.o: $(GAS_DIR)/solver/crossover/crossover.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_gas.o: $(GAS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Tabu search

main_tabu_search: $(OBJ_DIR)/qap.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/cl.o $(OBJ_DIR)/ts.o $(OBJ_DIR)/main_ts.o 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/ts.o: $(TS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_ts.o: $(TS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Utils

$(OBJ_DIR)/cl.o: $(CL)
	$(CC) $(CFLAGS) -c $< -o $@

# Memetic algorithm

main_memetic_algorithm: $(OBJ_DIR)/qap.o $(OBJ_DIR)/gas_mut.o $(OBJ_DIR)/gas_cros.o $(OBJ_DIR)/gas.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/mas.o $(OBJ_DIR)/main_mas.o
	$(CC) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/mas.o: $(MAS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_mas.o: $(MAS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean files

clean:
	rm  -f *.o $(OBJ_DIR)/*.o $(BIN_DIR)/*
