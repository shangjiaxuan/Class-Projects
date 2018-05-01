﻿#include "Array_2D.h"

using namespace std;

template<typename type>
Array_2D<type>::Array_2D(const int& row, const int& col) {
	this->data = new type[row*col];
	this->map = new type*[row];
	for (int i = 0; i < row; i++) {
		this->map[i] = this->data + i * col * sizeof(type);
	}
}

template<typename type>
Array_2D<type>::~Array_2D() {
	delete this->map;
	map = nullptr;
	delete this->data;
	data = nullptr;
}

template class Array_2D<char>;

