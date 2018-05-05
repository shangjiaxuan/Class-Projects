#include "step.h"

using namespace std;

void step::next_direction() {
	if (direction == up) {
		direction = ::right;
		return;
	}
	if (direction == ::right) {
		direction = down;
		return;
	}
	if (direction == down) {
		direction = ::left;
		return;
	}
	if (direction == ::left) {
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
