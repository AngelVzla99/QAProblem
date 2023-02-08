CC = g++
CFLAGS = -I -g -w -Wall -std=c++17
OBJ_DIR = objs
SRC_DIR = src
BIN_DIR = bin

QAP_DIR = $(SRC_DIR)/QAP
QAP = $(wildcard $(QAP_DIR)/*.cpp)

ES_DIR = $(SRC_DIR)/exact_solver
ES = $(wildcard $(ES_DIR)/solver/*.cpp)

LSS_DIR = $(SRC_DIR)/local_search_solver
LSS = $(wildcard $(LSS_DIR)/solver/*.cpp)

ILSS_DIR = $(SRC_DIR)/iterative_local_search_solver
ILSS = $(wildcard $(ILSS_DIR)/solver/*.cpp)

all: main_exact_solver main_local_search main_iterative_local_search main_benchmark

$(OBJ_DIR)/qap.o: $(QAP)
	$(CC) $(CFLAGS) -c $< -o $@

main_benchmark: $(OBJ_DIR)/qap.o $(OBJ_DIR)/es.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/ilss.o $(OBJ_DIR)/main_benchmark.o
	$(CC) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/main_benchmark.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

main_exact_solver: $(OBJ_DIR)/qap.o $(OBJ_DIR)/es.o $(OBJ_DIR)/main_es.o
	$(CC) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/es.o: $(ES) $(QAP)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_es.o: $(ES_DIR)/main.cpp $(ES)
	$(CC) $(CFLAGS) -c $< -o $@

main_local_search: $(OBJ_DIR)/qap.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/main_lss.o
	$(CC) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/lss.o: $(LSS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_lss.o: $(LSS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

main_iterative_local_search: $(OBJ_DIR)/qap.o $(OBJ_DIR)/lss.o $(OBJ_DIR)/ilss.o $(OBJ_DIR)/main_ilss.o 
	$(CC) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/ilss.o: $(ILSS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main_ilss.o: $(ILSS_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm  -f *.o $(OBJ_DIR)/*.o $(BIN_DIR)/*
