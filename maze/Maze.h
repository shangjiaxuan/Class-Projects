﻿#pragma once

#include "Cyclic_Queue.h"
#include "Array_2D.h"

constexpr char left{ 'L' };
constexpr char right{ 'R' };
constexpr char up{ 'U' };
constexpr char down{ 'D' };

struct step {
	size_t row;
	size_t col;
	char direction;
	void next_direction();
	void walk();
	step peek();
	bool operator==(step& st) {
		if(this->row==st.row&&this->col==st.col&&this->direction==st.direction) {
			return true;
		}
		return false;
	}
	void print(std::ostream& ost);
};

class Maze {
public:
	Maze();
	Cyclic_Queue<step>* Queue;
	Cyclic_Queue<step>* Route;
	Array_2D<char>* maze;
	step current;
	static size_t end_row;
	static size_t end_col;
	static size_t start_row;
	static size_t start_col;
	void look_around();
	bool direction_okay();
	void walk();
	bool Queue_okay();
	void pop_front();
	void find_route();
	void find_route_verbose(std::ostream& ost);
	void set_end(size_t row, size_t col) {
		end_row = row;
		end_col = col;
	}
	void set_start(size_t row, size_t col) {
		start_row = row;
		start_col = col;
	}
	bool finished() {
		if(current.row==end_row&&current.col==end_col) {
			return true;
		}
		return false;
	}
	void start() {
		current.row = start_row;
		current.col = start_col;
		current.direction = up;
	}
	void set_passed() {
		maze->map[current.row][current.col] = 2;
	}
	void reset_passed() {
		maze->map[current.row][current.col] = 0;
	}
	void parse_route(step added);
	void print_result();
	void solve_maze();
	void load_maze(std::istream& ist);
	void scan_maze_input(std::ifstream& ifs);
	std::string parse_path(std::string line);
};


