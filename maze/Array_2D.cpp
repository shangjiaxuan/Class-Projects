#include "Array_2D.h"

using namespace std;

template<typename type>
Array_2D<type>::Array_2D(const size_t& row, const size_t& col) {
	width = col;
	length = row;
	this->data = new type[row*col];
	this->map = new type*[row];
	for (size_t i = 0; i < row; i++) {
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

template<typename type> 
void Array_2D<type>::print_map_value(std::ostream & ost) {
	unsigned a;
	for(size_t i=0; i<length; i++) {
		for(size_t j=0; j<width; j++) {
			a = map[i][j];
			ost << a << '\t';
		}
		ost << '\n';
	}
	ost << endl;
}


template class Array_2D<char>;

