#pragma once

#include "Cyclic_Queue.h"
#include "Array_2D.h"
#include "Stack.h"

extern bool on;

class Maze {
public:
	Maze();
	void Interaction();
	static void name_ver();
	static bool on;
private:
	Array_2D<char>* maze;
	Cyclic_Queue* Queue;
	Stack* Route;
	step current;
	size_t end_row;
	size_t end_col;
	size_t start_row;
	size_t start_col;

	void solve_maze();
	void print_result();

	void start();
	void look_around();
	void pop_front();
	void walk();
	void parse_route(step added);
	void set_passed();
	void reset_passed();

	bool direction_okay();
	bool Queue_okay();
	bool finished();

	void find_route();
	void find_route_verbose(std::ostream& ost);

	void set_end(size_t row, size_t col);
	void set_start(size_t row, size_t col);

	void prompt();
	void load_maze(std::istream& ist);
	void scan_maze_input(std::ifstream& ifs);
	void conti();
	std::string parse_path(std::string line);
};


