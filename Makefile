# CC and CFLAGS are varilables
CC = g++ -std=c++11
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -DDEBUG
OPTFLAGS = -O2

all	: bin/sat_killer_sudoku
	@echo ""

run: bin/sat_killer_sudoku
	@echo ""
	@echo "Running optimized version..."
	@echo ""
	@bin/sat_killer_sudoku

# optimized version
bin/sat_killer_sudoku	: main.o killer_sudoku.o
	$(CC) $(OPTFLAGS) $^ -o $@

main.o	: src/main.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

killer_sudoku.o	: src/KillerSudoku.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

dbg	: bin/sat_killer_sudoku_dbg
	@echo ""

test: bin/sat_killer_sudoku_dbg
	@echo ""
	@echo "Running debug version..."
	@echo ""
	@bin/sat_killer_sudoku_dbg

bin/sat_killer_sudoku_dbg: main_dbg.o killer_sudoku_dbg.o
	$(CC) $(DBGFLAGS) $^ -o $@

main_dbg.o : src/main.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

killer_sudoku_dbg.o : src/KillerSudoku.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

# clean all the .o and executable files
clean:
	rm -rf *.o lib/*.a lib/*.o bin/*

