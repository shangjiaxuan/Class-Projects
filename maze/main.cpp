#include "Maze.h"

int main() {
	try {
		Maze This;
		std::cout << "The specified maze is:\n";
		This.maze->print_map_value(std::cout);
		This.solve_maze();
		This.print_result();
	}catch(std::exception& e) {
		std::cerr << e.what();
	}

	system("pause");
	return 0;
}
