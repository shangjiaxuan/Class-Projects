#pragma once

#include "Header.h"

struct index_block {
	int* data;
	size_t start;
	size_t bock_size;
	int& operator[](size_t i) {
		if (!(i<bock_size)) {
			throw std::runtime_error("int& index_block::operator[](size_t): index too large!");
		}
		return data[start + i];
	}
};

class search {
	search();
	~search();
	size_t block_num;
	index_block* index;
	int* data;
};

inline std::string parse_path(std::string line) {
	std::string path;
	std::istringstream iss(line);
	char current;
	iss.get(current);
	while (current == ' ') {
		iss.get(current);
	}
	iss.putback(current);
	while (iss) {
		iss.get(current);
		switch (current) {
		case '\"':
			iss.get(current);
			while (current != '\"') {
				path.push_back(current);
				iss.get(current);
			}
			break;
		default:
			path.push_back(current);
			iss.get(current);
		}
	}
	return path;
}
