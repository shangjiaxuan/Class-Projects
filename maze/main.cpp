#include "Maze.h"

int main() {
	Maze::name_ver();
	while(Maze::on) {
		try {
			Maze This;
			This.Interaction();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << '\n' << std::endl;
		}
	}
	return 0;
}
