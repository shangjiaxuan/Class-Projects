#pragma once

#include "Cyclic_Queue.h"
#include "Array_2D.h"

class Maze : public Array_2D<char> {
	Cyclic_Queue<step> Queue{10000};
	
};