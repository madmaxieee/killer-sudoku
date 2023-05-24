# CC and CFLAGS are varilables
CC = g++ -std=c++20 -Wno-deprecated-declarations
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -DDEBUG -fsanitize=address
OPTFLAGS = -O3

all	: bin/sat_killer_sudoku
	@echo ""

run: bin/sat_killer_sudoku
	@echo ""
	@echo "Running optimized version..."
	@echo ""
	@bin/sat_killer_sudoku

# optimized version
bin/sat_killer_sudoku	: bin/main.o bin/KillerSudoku.o bin/KSSolver.o bin/PBSatSolver.o bin/Solver.o bin/File.o bin/Proof.o
	$(CC) $(OPTFLAGS) $^ -o $@

bin/main.o	: src/main.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

bin/KillerSudoku.o	: src/KillerSudoku.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

bin/KSSolver.o	: src/KSSolver.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

bin/PBSatSolver.o	: src/PBSatSolver.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

bin/File.o	: src/File.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

bin/Proof.o	: src/Proof.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

bin/Solver.o	: src/Solver.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

dbg	: bin/sat_killer_sudoku_dbg
	@echo ""

test: bin/sat_killer_sudoku_dbg
	@echo ""
	@echo "Running debug version..."
	@echo ""
	@bin/sat_killer_sudoku_dbg

bin/sat_killer_sudoku_dbg: bin/main_dbg.o bin/KillerSudoku_dbg.o bin/KSSolver_dbg.o bin/PBSatSolver_dbg.o bin/Solver_dbg.o bin/File_dbg.o bin/Proof_dbg.o
	$(CC) $(DBGFLAGS) $^ -o $@

bin/main_dbg.o : src/main.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

bin/KillerSudoku_dbg.o : src/KillerSudoku.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

bin/KSSolver_dbg.o : src/KSSolver.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

bin/PBSatSolver_dbg.o : src/PBSatSolver.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

bin/File_dbg.o : src/File.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

bin/Proof_dbg.o : src/Proof.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

bin/Solver_dbg.o : src/Solver.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

# clean all the bin/.o and executable files
clean:
	rm -rf *.o lib/*.a lib/*.o bin/*
