#pragma once

#include "Header.h"

template<typename type>
class Array_2D {
public:
	Array_2D(const size_t& row, const size_t& col);
	Array_2D() {
		throw std::runtime_error("Array_2D: cannot initialize without two integers!");
	}
	~Array_2D();
//	type* get(int i, int j);
//	void write(type* pos, type val);
	virtual void init() {};
	static size_t length;
	static size_t width;
	type** map;
//protected:
	type* data;
	void print_map_value(std::ostream& ost);
};
