#include "Booklist.h"

using namespace std;

list::found list::find(int index) {
	if (!head) {
		return { 'N', nullptr , nullptr };
	}
	if (index < head->index_number) {
		return { 'S' , nullptr, head };
	}
	volume* current = head;
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

list::found list::find(std::string name) {
	if (!head) {
		return { 'N', nullptr , nullptr };
	}
	volume* current = head;
	volume* traceback = nullptr;
	if (head->name != name) {
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

void list::add(volume* previous, int index, std::string name, bool index_type) {
	volume* current = new volume;
	current->index_number = index;
	current->fixed_index = index_type;
	current->name = name;
	current->next = previous->next;
	previous->next = current;
}




void Book::init_ntoken() {
	int size = default_non_tokens.size();
	for (int i = 0; i < size; i++) {
		index.add_token(default_non_tokens[i])->head->index_number = -1;
	}
}

volume* Book::reindex(list::found original) {
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

//void Book::add(volume* previous, int index, std::string name, bool index_type) {
//	volume* current = new volume;
//	current->index_number = index;
//	current->fixed_index = index_type;
//	current->name = name;
//	current->next = previous->next;
//	previous->next = current;
//}

bool Book::add(std::string name) {
	To_standard(name);
	list::found temp = booklist.find(name);
	switch (temp.status) {
	case 'P':
		return false;
	case 'N':
		booklist.head = new volume;
		booklist.head->name = name;
		booklist.head->fixed_index = false;
		booklist.head->index_number = 0;
		booklist.head->next = nullptr;
		{vector<string> tokens = get_tokens(name);
		int size = tokens.size();
		for (int i = 0; i < size; i++) {
			this->index.add_token(tokens[i])->add(0);
		}
		}
		return true;
	case 'X':
	{
		if (booklist.head->index_number != 0) {
			volume* add = new volume;
			add->next = booklist.head;
			add->index_number = 0;
			add->name = name;
			add->fixed_index = false;
			booklist.head = add;
			{vector<string> tokens = get_tokens(name);
			int s = tokens.size();
			for (int i = 0; i < s; i++) {
				this->index.add_token(tokens[i])->add(0);
			}
			}
			return true;
		}
		int index = 1;
		volume* xconti = booklist.head->next;
		volume* track = booklist.head;
		while (xconti->index_number == index) {
			track = xconti;
			xconti = xconti->next;
			index++;
		}
		booklist.add(track, index, name, false);
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
	list::found temp = booklist.find(index);
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
			booklist.add(reindex(temp), index, name, true);
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
		booklist.head = new volume;
		booklist.head->name = name;
		booklist.head->fixed_index = true;
		booklist.head->index_number = index;
		booklist.head->next = nullptr;
		}
		return true;
	case 'S':
		{
		volume* newh = new volume;
		newh->name = name;
		newh->index_number = index;
		newh->fixed_index = true;
		newh->next = booklist.head->next;
		booklist.head = newh;
		return true;
		}
	case 'L': case 'M':
	{
		booklist.add(temp.rtn, index, name, true);
		return true;
	}
	default:
	{
		throw runtime_error("add(int): Unkown found status!");
	}
	}
}

bool Book::del(int index) {
	list::found temp = booklist.find(index);
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
	list::found temp = booklist.find(name);
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
	index_input.open("index.dat");
	if(index_input) {
		index.load(index_input);
	}
	index_input.close();
	booklist_input.open("booklist.dat");


	booklist_input.close();
}


//保存现有书目和词索引表的内容到文件
//书目：以 '\n' 表示书目名称结束
//字典树（按老师说法貌似是256叉树了。。。）：
//节点数据内容：以"\0{\0" 和 "\0}\0" 包围书目编号（数字），然后每个数字后用一个空格隔开
//节点之间的关系按存先根遍历的方法输出到文件，每次到达末端回复到父节点时输出一个空字符表示此逻辑关系
//先输出每个节点数据（书编号链表和大括号）再输出空字符或者子节点
//没有书号链表的不输出大括号
void Book::save() {
	index_output.open("index.dat");
	if(!index_output) {
		throw runtime_error("Book::save: cannot open 'index.dat' for writing!");
	}
	index.save(index_output);
	index_output.close();
	booklist_output.open("booklist.dat");
	if(booklist.head) {
		volume* current = booklist.head;
		while(true) {
			cout << current->index_number << ' ';
			if(current->fixed_index) {
				cout << "t ";
			}else {
				cout << "f ";
			}
			cout << current->name << ' ';
			if(!current->next) {
				break;
			}
		}
	}
	booklist_output.close();
}

/*
void Book::save_loop(ofstream& ofs, node* current) {
	if (current->head) {
		cout << "\0{\0";
		item* book = current->head;
		do {
			cout << book->index_number << '\0';
		} while (book->next);
		cout << book->index_number;
		cout << "\0}\0";
	}
	for(unsigned char i=0; i<CharNum; i++) {
		if(current->next[i]) {
			cout << reinterpret_cast<char&>(i);
			save_loop(ofs, current->next[i]);
		}
	}
}*/


void print_tokens_loop(node* current, string token) {
	if(current->head) {
		if (current->head->index_number >= 0) {
			cout << token << endl;
		}
	}
	for(unsigned i=0; i<CharNum; i++) {
		if(current->next[i]) {
			token.push_back(reinterpret_cast<char&>(i));
			print_tokens_loop(current->next[i], token);
			token.pop_back();
		}
	}
}

void Book::print_tokens() {
	string token="";
	print_tokens_loop(const_cast<node*>(&index.head), token);
}
