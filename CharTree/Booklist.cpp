#include "Booklist.h"

using namespace std;

void Book::init_ntoken() {
	int size = default_non_tokens.size();
	for (int i = 0; i < size; i++) {
		index.add_token(default_non_tokens[i])->head->index_number = -1;
	}
}



volume* Book::reindex(Book::found original) {
	const int oindex = original.rtn->index_number;
	volume* traceback= original.rtn;
	volume* cur = traceback->next;
	int index = original.rtn->index_number + 1;
	while(index==cur->index_number) {
		traceback = cur;
		cur = cur->next;
		index++;
	}
	original.traceback->next = original.rtn->next;
	original.rtn->index_number = index;
	original.rtn->next = cur;
	traceback->next = original.rtn;
	//找出所有关键词，用关键词访问书目链表，重命名所有oindex为index
	vector<string> tokens = get_tokens(original.name);
	int s = tokens.size();
	for(int i=0;i<s;i++) {
		if(item* temp = this->index.locate(tokens[i])->head) {
			while (temp->index_number != oindex) {
				if (!temp->next) {
					throw runtime_error("Book::reindex: cannot find original index in a certain token");
				}
				temp = temp->next;
			}
			temp->index_number = index;
		}else {
			throw runtime_error("Book::reindex: token not found!");
		}
	}
	return original.traceback;
}

Book::found Book::find(int index) {
	if (booklist_head->index_number==-2) {
		return { 'N', nullptr , nullptr };
	}
	if (index < booklist_head->index_number) {
		return { 'S' , nullptr, booklist_head };
	}
	volume* current = booklist_head;
	volume* traceback = nullptr;
	while (index > current->index_number) {
		if (current->next == nullptr) {
			return { 'M' , current, nullptr };
		}
		traceback = current;
		current = current->next;
	}
	if (index == current->index_number) {
		return { 'P' , current, traceback, current->name };
	}
	return { 'L' , nullptr, traceback };
}

Book::found Book::find(std::string name) {
	if (booklist_head->index_number==-2) {
		return { 'N', nullptr , nullptr };
	}
	volume* current = booklist_head;
	volume* traceback = nullptr;
	if (booklist_head->name != name) {
		while (current->next != nullptr) {
			traceback = current;
			current = current->next;
			if (current->name == name) {
				break;
			}
		}
	}
	if (current->name == name) {
		return { 'P' , current, traceback, current->name };
	}
	return { 'X' , nullptr , nullptr };
}

void Book::add(volume* previous, int index, std::string name, bool index_type) {
	volume* current = new volume;
	current->index_number = index;
	current->fixed_index = index_type;
	current->name = name;
	current->next = previous->next;
	previous->next = current;
}

bool Book::add(std::string name) {
	To_standard(name);
	found temp = find(name);
	switch (temp.status) {
	case 'P':
		return false;
	case 'N':
		booklist_head->name = name;
		booklist_head->fixed_index = false;
		booklist_head->index_number = 0;
		booklist_head->next = nullptr;
		{vector<string> tokens = get_tokens(name);
		int size = tokens.size();
		for (int i = 0; i < size; i++) {
			this->index.add_token(tokens[i])->add(0);
		}
		}
		return true;
	case 'X':
	{
		if (booklist_head->index_number != 0) {
			volume* add = new volume;
			add->next = booklist_head;
			add->index_number = 0;
			add->name = name;
			add->fixed_index = false;
			booklist_head = add;
			{vector<string> tokens = get_tokens(name);
			int s = tokens.size();
			for (int i = 0; i < s; i++) {
				this->index.add_token(tokens[i])->add(0);
			}
			}
			return true;
		}
		int index = 1;
		volume* xconti = booklist_head->next;
		volume* track = booklist_head;
		while (xconti->index_number == index) {
			track = xconti;
			xconti = xconti->next;
			index++;
		}
		add(track, index, name, false);
		{vector<string> tokens = get_tokens(name);
		int size = tokens.size();
		for (int i = 0; i < size; i++) {
			this->index.add_token(tokens[i])->add(index);
		}
		}
		return true;
	}
	default:
	{
		throw std::runtime_error("Book::add: Unkown found status!");
	}
	}
}

bool Book::add(int index, std::string name) {
	To_standard(name);
	found temp = find(index);
	switch(temp.status) {
	case 'P':
	{
		if (temp.rtn->fixed_index) {
			if (temp.rtn->name == name) {
				vector<string> tokens = get_tokens(name);
				int size = tokens.size();
				for(int i=0; i<size; i++) {
					this->index.add_token(tokens[i])->add(index);
				}
				return true;
			}
			return false;
		}
		else {
			add(reindex(temp), index, name, true);
			vector<string> tokens = get_tokens(name);
			int size = tokens.size();
			for (int i = 0; i<size; i++) {
				this->index.add_token(tokens[i])->add(index);
			}
			return true;
		}
	}
	case 'N':
		{
		booklist_head->name = name;
		booklist_head->fixed_index = true;
		booklist_head->index_number = index;
		booklist_head->next = nullptr;
		}
		return true;
	case 'S':
		{
		volume* newh = new volume;
		newh->name = name;
		newh->index_number = index;
		newh->fixed_index = true;
		newh->next = booklist_head->next;
		booklist_head = newh;
		return true;
		}
	case 'L': case 'M':
	{
		add(temp.rtn, index, name, true);
		return true;
	}
	default:
	{
		throw runtime_error("add(int): Unkown found status!");
	}
	}
}

bool Book::del(int index) {
	found temp = find(index);
	switch(temp.status) {
	case 'P':
		temp.traceback->next = temp.rtn->next;
		delete[] temp.rtn;
		temp.rtn = nullptr;
		{string name = temp.name;
		vector<string> tokens = get_tokens(name);
		int size = tokens.size();
		for (int i = 0; i<size; i++) {
			this->index.del_token(tokens[i]);
		}
		}
		return true;
	case 'N': case 'L': case 'M': case 'S':
		return false;
	default:
		throw runtime_error("del(int): Unknown status!");
	}
}

bool Book::del(std::string name) {
	To_standard(name);
	found temp = find(name);
	switch (temp.status) {
	case 'P':
		if(temp.traceback) {
			temp.traceback->next = temp.rtn->next;
		}
		temp.rtn->~volume();
		{vector<string> tokens = get_tokens(name);
		int size = tokens.size();
		for(int i=0; i<size; i++) {
			this->index.del_token(tokens[i]);
		}
		}
		temp.rtn = nullptr;
		return true;
	case 'N': case 'X':
		return false;
	default:
		throw runtime_error("del(string): Unknown status!");
	}
}


//有大写又有小写的规范很复杂，要考虑词的性质和长度，还有“I”一类的词，干脆英文全大写（尽管完全和词相关，和位置关系不大（除了第一个词））
void Book::To_standard(std::string& bookname) {
	const int size = bookname.size();
	for(int i=0; i<size; i++) {
		if (bookname[i]>96 && bookname[i]<123) {
			bookname[i] -= 32;
		}
	}
}

std::vector<std::string> Book::get_tokens(std::string bookname) {
	vector<string> rtn;
	string current;
	istringstream str(bookname);
	while(str>>current) {
		if(istoken(current)) {
			rtn.push_back(current);
		}
	}
	return rtn;
}

bool Book::istoken(std::string token) {
	node* temp = index.locate(token);
	if(!temp) {
		return true;
	}
	if(temp->head->index_number==-1) {
		return false;
	}
	return true;
}

void Book::load(){
	index_input.open("index.dat",ios::binary);
	booklist_input.open("booklist.dat",ios::binary);
	while() {

	}




	index_input.close();
	booklist_input.close();
}

void Book::save() {
	index_output.open("index.dat", ios::binary);
	booklist_output.open("booklist.dat", ios::binary);
	


	index_output.close();
	booklist_output.close();
}
