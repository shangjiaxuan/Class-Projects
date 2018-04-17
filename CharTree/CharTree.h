#pragma once

#include "Header.h"

#define CharNum 256

struct item {
	~item() {
		if(next){
			delete next;
			next = nullptr;
		}
	}
	int index_number;
	item* next;
};

struct node {				//字符节点
	node();
	~node();
	node** next;
	item* head;
	//以后考虑改写代码好看些试试
	node*& operator [] (size_t elem) {
		return *&next[elem];
	}
	void add(int index) {
		item* cur = new item;
		cur->next = head;
		cur->index_number = index;
		head = cur;
	}
};


class CharTree {			//字符树的头和操作
public:
//	CharTree() = default;
//	~CharTree();
	node head;
	node* locate(std::string token);
	item* access(std::string token);
	node* add_token(std::string token);
	bool one_succ(node** list);
	bool del_token(std::string token);
	void save(ofstream& ofs);
	void load(ifstream& ifs);
};
