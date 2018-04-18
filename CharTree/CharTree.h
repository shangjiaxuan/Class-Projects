#pragma once

#include "Header.h"

#define CharNum 256

struct item {
	~item() {
		if(next_item){
			next_item->~item();
			delete next_item;
			next_item = nullptr;
		}
	}
	int index_number;
	item* next_item{ nullptr };
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
		cur->next_item = head;
		cur->index_number = index;
		head = cur;
	}
};


class CharTree {			//字符树的头和操作
public:
//	CharTree() = default;
//	~CharTree();
	const node head;
	node* locate(std::string token);
	item* access(std::string token);
	node* add_token(std::string token);
	bool one_succ(node** list);
	bool del_token(std::string token);
	void save(std::ofstream& ofs);
	void save_loop(node* current, std::ofstream& ofs);
	void load(std::ifstream& ifs);
	void load_loop(node* current, std::ifstream& ifs);
	void load_loop_start(std::ifstream& ifs);
	void item_list(node* current, std::ifstream& ifs);

	bool node_found;
	std::string find_node(node* target);
	void find_node_loop(node* target, node* current, std::string& token);

	void print_tokens();
	void print_tokens_loop(node* current, std::string& token);
};
