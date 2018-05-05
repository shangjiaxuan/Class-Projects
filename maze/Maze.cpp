#include "Maze.h"

using namespace std;

template class Cyclic_Queue<step>;

void Maze::look_around() {
	current.direction = up;
	do {
		if (direction_okay()&&Queue_okay()) {
			Queue->push_back(current);
		}
		current.next_direction();
	} while (current.direction!=::left);
}

void step::next_direction() {
	if(direction==up) {
		direction = ::right;
		return;
	}
	if(direction==::right) {
		direction = down;
		return;
	}
	if(direction==down) {
		direction = ::left;
		return;
	}
	if(direction==::left) {
		direction = up;
		return;
	}
	throw runtime_error("step::next_direction(): Unknown direction!");
}

void step::walk() {
	if (direction == up) {
		row--;
	}
	if (direction == ::right) {
		col++;
	}
	if (direction == down) {
		row++;
	}
	if (direction == ::left) {
		col--;
	}
	direction = up;
}

step step::peek() {
	step rtn = *this;
	if (direction == up) {
		rtn.row--;
	}
	if (direction == ::right) {
		rtn.col++;
	}
	if (direction == down) {
		rtn.row++;
	}
	if (direction == ::left) {
		rtn.col--;
	}
	rtn.direction = up;
	return rtn;
}


//true only if the location headed is a okay path and not covered before
bool Maze::direction_okay() {
	if(current.direction==up) {
		if (current.row==0) {
			return false;
		}
		return !(maze->map[current.row - 1][current.col]);
	}
	if(current.direction==down) {
		if(current.row==maze->length-1) {
			return false;
		}
		return !(maze->map[current.row + 1][current.col]);
	}
	if(current.direction==::left) {
		if(current.col==0) {
			return false;
		}
		return !(maze->map[current.row][current.col - 1]);
	}
	if(current.direction==::right) {
		if(current.col==maze->width-1) {
			return false;
		}
		return !(maze->map[current.row][current.col + 1]);
	}
	throw runtime_error("Maze::direction_okay(): Unknown direction!");
}

void Maze::walk() {
	parse_route(current);
	current.walk();
	set_passed();
}

bool Maze::Queue_okay() {
	size_t i = Queue->start;
	step next = current.peek();
	do {
		if (Queue->data[i].peek()==next) {
			return false;
		}
		i++;
		if (i >= Queue->total_size) {
			i -= Queue->total_size;
		}
	} while (i != Queue->end);
	return true;
}

void Maze::pop_front() {
	current = Queue->pop_front();
}

void Maze::find_route() {
	start();
	set_passed();
	while(!finished()) {
		look_around();
		pop_front();
		walk();
	}
}

void Maze::find_route_verbose(std::ostream& ost) {
	start();
	set_passed();
	ost << "Current maze is:\n";
	maze->print_map_value(ost);
	while (!finished()) {
		ost << "Finding directions that are okay...\n";
		look_around();
		ost << "Queue is now:\n";
		Queue->print(ost);
		ost << "Setting new current state...\n";
		pop_front();
		ost << "Current state is now:\n";
		current.print(ost);
		ost << "Walking according to direction...\n";
		walk();
		ost << "Current state is now:\n";
		current.print(ost);
		ost << "Current maze is:\n";
		maze->print_map_value(ost);
		ost << "Current path is:\n";
		Route->print(ost);
	}
}



void Maze::parse_route(step added) {
	size_t size = Route->size();
	size_t end{ 0 };
	for(size_t i=size-1; i>=0; i--) {
		if ((*Route)[i].row == added.row && (*Route)[i].col == added.col) {
			end = i + 1;
			break;
		}
	}
	for(size_t i=0; i<end; i++) {
		Route->pop_front();
	}
	Route->push_back(added);
}

void step::print(std::ostream& ost) {
	ost << "Row:\t" << row << '\n';
	ost << "Column:\t" << col << '\n';
	ost << "Direction:\t";
	if (direction == up) {
		ost << "up\n";
	}
	else if (direction == ::right) {
		ost << "right\n";
	}
	else if (direction == down) {
		ost << "down\n";
	}
	else if (direction == ::left) {
		ost << "left\n";
	}
	ost << endl;
}

void Maze::print_result() {
	Route->print(cout);
}

void Maze::solve_maze() {
	find_route_verbose(cout);
	print_result();
}

void Maze::load_maze(std::istream& ist) {
	
}

