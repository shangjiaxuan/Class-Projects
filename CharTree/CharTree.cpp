#include "CharTree.h"
#include "Booklist.h"

node::node() {
	next = new node*[CharNum];
  for (int i; i<CharNum; i++){
    next[i] = nullptr;
  }
	head = nullptr;
}

node::~node() {
	for (int i = 0; i < CharNum; i++) {
		if(next[i]) {
			delete next[i];
			next[i] = nullptr;
		}
	}
	//就是怕栈溢出。。。递归调用。。。
	delete[] next;
	next = nullptr;
  if(head){
    	delete[] head;
      head = nullptr;
  }
}

node* CharTree::locate(std::string token) {
	const int size = token.size();
	node* current = &head;
	for(int i=0; i<size; i++) {
		if(current->next[reinterpret_cast<const unsigned char&>(token[i])]) {
			current = current->next[reinterpret_cast<const unsigned char&>(token[i])];
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
	node* current = &head;
	for (int i = 0; i < size; i++) {
		if(current->next[reinterpret_cast<const unsigned char&>(token[i])]!=nullptr) {
			current = current->next[reinterpret_cast<const unsigned char&>(token[i])];
		} else {
			current->next[reinterpret_cast<const unsigned char&>(token[i])] = new node;
			current = current->next[reinterpret_cast<const unsigned char&>(token[i])];
		}
	}
	if(!current->head) {
		current->head = new item;
		current->head->next = nullptr;
	}
	return current;
}

bool CharTree::del_token(std::string token) {
	int size = token.size();
	node* current = &head;
	//see if the character is the only one and the branch can be removed
	bool* remove = new bool[size];
	for (int i = 0; i<size; i++) {
		if (current->next[reinterpret_cast<const unsigned char&>(token[i])]) {
			if(one_succ(current->next)) {
				remove[i] = true;
			}
			current = current->next[reinterpret_cast<const unsigned char&>(token[i])];
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
	delete[] temp->next[reinterpret_cast<const unsigned char&>(common.back())];
	temp->next[reinterpret_cast<const unsigned char&>(common.back())] = nullptr;
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

void CharTree::save(ofstream& ofs){
  node* current = &head;
  save_loop(current, ofs);
}

void CharTree::save_loop(node* current, ofstream& ofs){
  for (unsigned char i=0; i<CharNum; i++){
    if(current->next[i]){
      ofs << reinterpret_cast<char>(i);
      save_loop(next[i], ofs);
    }
    else{
      ofs << '\t';
    }
    if(current->head){
      ofs<<'{'
      item* label = current->head;
      do {
        ofs<<label->index_number<<' ';
      } while(label->next);
      ofs<<'}'
    }
  }
}

void CharTree::load(ifstream& ifs){
  char temp;
  node* current = &head;
  load_loop(current, ifs);
}

void CharTree::load_loop(node* current, ifstram& ifs){
  ifs.get(temp);
  switch(temp){
    case '\t':
      if(ifs.peek()=='{'){
        item_list(current,ifs);
      }
      break;
    default:
      next[reinterpret_cast<unsigned char>(temp)] = new node;
      load_loop(next[reinterpret_cast<unsigned char>(temp)], ifs);
  }
}

void CharTree::item_list(node* current, ifstram& ifs){
  int index_number;
  item* cur = current->head;
  char temp;
  ifs.get(temp);
  while(temp!='}'){
    ifs.putback(temp);
    ifs>>index_number;
    cur->index_number=index_number;
    ifs.get();
    ifs.get(temp);
    if(isdigit(temp)){
      cur->next=new item;
      cur=cur->next;
      ifs.putback(temp);
    }
    else{
      cur->next=nullptr;
      break;
    }
  }
  break;
}
