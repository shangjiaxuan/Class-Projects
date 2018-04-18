#include "CharTree.h"
#include "Booklist.h"

using namespace std;

node::node() {
	next = nullptr;
	head = nullptr;
}

node::~node() {
	if(next) {
		for(int i = 0; i < CharNum; i++) {
			if(next[i]) {
				delete next[i];
				next[i] = nullptr;
			}
		}
		//就是怕栈溢出。。。递归调用。。。
		delete[] next;
		next = nullptr;
	}
	if(head) {
		delete head;
		head = nullptr;
	}
}

node* CharTree::locate(std::string token) {
	const int size = token.size();
	node* current = const_cast<node*>(&head);
	unsigned char loc;
	if(size==0) {
		return const_cast<node*>(&head);
	}
	for(int i=0; i<size; i++) {
		loc = token[i];
		if(current->next[loc]) {
			current = current->next[loc];
		}else {
			return nullptr;
		}
	}
	return current;
}

item* CharTree::access(std::string token) {
	return locate(token)->head;
}

node* CharTree::add_token(std::string token) {
	const int size = token.size();
	node* current = const_cast<node*>(&head);
	unsigned char loc;
	for (int i = 0; i < size; i++) {
		loc = token[i];
		if(!current->next) {
			current->next = new node*[CharNum];
			for (int i = 0; i < CharNum; i++) {
				current->next[i] = nullptr;
			}
			current->head = nullptr;
		}
		if(current->next[loc]!=nullptr) {
			current = current->next[loc];
		} else {
			current->next[loc] = new node;
			current = current->next[loc];
		}
	}
//	if(!current->head) {
//		current->head = new item;
//		current->head->next_item = nullptr;
//		current->head->index_number = -2;
//	}
	return current;
}

bool CharTree::del_token(std::string token) {
	int size = token.size();
	node* current = const_cast<node*>(&head);
	//see if the character is the only one and the branch can be removed
	bool* remove = new bool[size]();
	unsigned char loc;
	for (int i = 0; i<size; i++) {
		loc = token[i];
		if (current->next[loc]) {
			if(one_succ(current->next)) {
				remove[i] = true;
			}
			current = current->next[loc];
		}
		else {
			return false;
		}
	}
	//找出只有这个节点的开头
	size--;
	while(remove[size]) {
		size--;
	}
	std::string common = token.substr(0, size);
	node* temp = locate(common);
	loc = token[size];
	if(temp->next) {
		delete (temp->next[loc]);
		temp->next[loc] = nullptr;
	}
	delete[] remove;
	remove = nullptr;
	return true;
}

bool CharTree::one_succ(node** list) {
	bool seen{ false };
	for(int i=0; i<CharNum; i++) {
		if(list[i]) {
			if(seen) {
				return false;
			}
			seen = true;
		}
	}
	if(!seen) {
		throw std::runtime_error("CharTree::one_succ: No successor found!");
	}
	return true;
}


void CharTree::save(ofstream& ofs) {
	node* current = const_cast<node*>(&head);
	save_loop(current, ofs);
}

void CharTree::save_loop(node* current, ofstream& ofs) {
	char char_buffer;
	if (current->head) {
		if (current->head->index_number<0) {
			return;
		}
		ofs << '{';
		item* label = current->head;
		ofs << label->index_number << ' ';
		while (label->next_item) {
			label = label->next_item;
			ofs << label->index_number << ' ';
		}
		ofs << '}';
	}
	if (!current->next) {
		return;
	}
	for(unsigned i = 0; i < CharNum; i++) {
		if(current->next[i]) {
			char_buffer = i;
			ofs << char_buffer;
			save_loop(current->next[i], ofs);
			ofs << '\t';
		}
	}
}

void CharTree::load(ifstream& ifs) {
	node* start = const_cast<node*>(&head);
	if (!start->next) {
		start->next = new node*[CharNum];
		for (int i = 0; i < CharNum; i++) {
			start->next[i] = nullptr;
		}
		start->head = nullptr;
	}
	load_loop_start(ifs);
}

void CharTree::load_loop_start(std::ifstream& ifs) {
	while(!ifs.eof()) {
		load_loop(const_cast<node*>(&head), ifs);
	}
}


void CharTree::load_loop(node* current, ifstream& ifs) {
	char temp;
	if(ifs.eof()) {
		return;
	}
	ifs.get(temp);
	unsigned char loc = temp;
	if(temp=='\t') {
		return;
	} 
	else if (temp=='{') {
		item_list(current, ifs);
		load_loop(current, ifs);
		return;
	}
	else {
		if (!current->next) {
			current->next = new node*[CharNum];
			for (int i = 0; i < CharNum; i++) {
				current->next[i] = nullptr;
			}
			current->head = nullptr;
		}
		current->next[loc] = new node;
		load_loop(current->next[loc], ifs);
		return;
	}
/*	switch(temp) {
		case '\t':
			if(ifs.peek() == '{') {
				item_list(current, ifs);
			}
			break;
		default:
			if(!current->volume_next) {
				current->volume_next = new node*[CharNum];
				for (int i = 0; i < CharNum; i++) {
					current->volume_next[i] = nullptr;
				}
				current->head = nullptr;
			}
			current->volume_next[loc] = new node;
			load_loop(current->volume_next[loc], ifs);
	}*/
}

void CharTree::item_list(node* current, ifstream& ifs) {
	int index_number;
	current->head = new item;
	item* cur = current->head;
	char temp;
	ifs.get(temp);
	while(temp != '}') {
		ifs.putback(temp);
		ifs >> index_number;
		cur->index_number = index_number;
		ifs.get();
		ifs.get(temp);
		if(isdigit(temp)) {
			cur->next_item = new item;
			cur = cur->next_item;
			ifs.putback(temp);
		} else if(temp=='}') {
			cur->next_item = nullptr;
			break;
		} else {
			throw  runtime_error("CharTree::item_list: index expected!");
		}
	}
	// break;
}

void CharTree::find_node_loop(node* target, node* current, string& token) {
	unsigned char temp;
	char char_buff;
	for(unsigned i=0; i<CharNum; i++) {
		if(!current->next) {
			return;
		}
		if (current->next[i]) {
			temp = i;
			char_buff = temp;
			token.push_back(char_buff);
			if (current->next[i] == target) {
				node_found = true;
				return;
			}
			find_node_loop(target, current->next[i], token);
			if(node_found) {
				return;
			}
			token.pop_back();
		}
	}
}

string CharTree::find_node(node* target) {
	string token;
	node_found = false;
	find_node_loop(target, const_cast<node*>(&head), token);
	return token;
}

void CharTree::print_tokens_loop(node* current, string& token) {
	if (current->head) {
		if (current->head->index_number >= 0) {
			cout << token << endl;
		}
	}
	for (unsigned i = 0; i<CharNum; i++) {
		if (!current->next) {
			return;
		}
		if (current->next[i]) {
			token.push_back(reinterpret_cast<char&>(i));
			print_tokens_loop(current->next[i], token);
			token.pop_back();
		}
	}
}

void CharTree::print_tokens() {
	string token = "";
	print_tokens_loop(const_cast<node*>(&(this->head)), token);
}
