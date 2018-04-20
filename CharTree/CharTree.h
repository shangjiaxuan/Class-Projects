#pragma once

#include "Header.h"

#define CharNum 256

struct item {
	~item() {
		if(next_item){
//			next_item->~item();
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
		item* added = new item;
		added->index_number = index;
		item* current = head;
		item* traceback = nullptr;
		while(current) {
			if(current->index_number==index) {
				throw std::runtime_error("node::add: index exists!");
			}
			if(current->index_number>index) {
				if(!traceback) {
					added->next_item = current;
					head = added;
					return;;
				}
				else {
					added->next_item = current;
					traceback->next_item = added;
					return;
				}
			}
			if (!current->next_item) {
				current->next_item = added;
				return;
			}
			traceback = current;
			current = current->next_item;
		}
		head = added;
	}
	void del(int index) {
		item* current = head;
		item* traceback = nullptr;
		while(current) {
			if (current->index_number == index) {
				traceback->next_item = current->next_item;
				delete current;
				current = nullptr;
				return;
			}
			if(current->index_number>index||!current->next_item) {
				throw std::runtime_error("node::del: cannot find specified index!");
			}
			traceback = current;
			current = current->next_item;
		}
		throw std::runtime_error("node::del: no booklist exists for specified token!");
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
