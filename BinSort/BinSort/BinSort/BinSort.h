#pragma once

#include "Header.h"

std::string parse_path(std::string line);

struct node {
	int data;
	size_t lchild;
	size_t rchild;
	size_t parent;
	size_t depth;
};

class BinSort {
	BinSort();
	node* data;
	size_t root;
};



