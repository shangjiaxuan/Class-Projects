#include "Booklist.h"

using namespace std;

list::found list::find(int index) {
	if (!head) {
		return { 'N', nullptr , nullptr };
	}
	if (index < head->index_number) {
		return { 'S' , nullptr, head->volume_next };
	}
	volume* current = head;
	volume* traceback = nullptr;
	while (index > current->index_number) {
		if (current->volume_next == nullptr) {
			return { 'M' , current, nullptr };
		}
		traceback = current;
		current = current->volume_next;
	}
	if (index == current->index_number) {
		return { 'P' , current, traceback, current->name };
	}
	return { 'L' , nullptr, traceback->volume_next };
}

list::found list::find(std::string name) {
	if (!head) {
		return { 'N', nullptr , nullptr };
	}
	volume* current = head;
	volume* traceback = nullptr;
	if (head->name != name) {
		while (current->volume_next != nullptr) {
			traceback = current;
			current = current->volume_next;
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
	if (!previous) {
		current->volume_next = head;
		head = current;
		return;
	}
	current->volume_next = previous->volume_next;
	previous->volume_next = current;
}




void Book::init_ntoken() {
	int size = default_non_tokens.size();
	for (int i = 0; i < size; i++) {
		node* added = index.add_token(default_non_tokens[i]);
		if(!added->head) {
			added->head = new item;
		}
		added->head->index_number = -1;
	}
}

volume* Book::reindex(list::found original) {
	const int oindex = original.rtn->index_number;
	volume* traceback= original.rtn;
	volume* cur = traceback->volume_next;
	int index = original.rtn->index_number + 1;
	while(cur->fixed_index&&index==cur->index_number) {
		traceback = cur;
		cur = cur->volume_next;
		index++;
		if(!cur) {
			break;
		}
	}
	original.traceback = original.rtn->volume_next;
	original.rtn->index_number = index;
	original.rtn->volume_next = cur;
	traceback->volume_next = original.rtn;
	//找出所有关键词，用关键词访问书目链表，重命名所有oindex为index
	vector<string> tokens = get_tokens(original.name);
	int s = tokens.size();
	for(int i=0;i<s;i++) {
		if(item* temp = this->index.locate(tokens[i])->head) {
			while (temp->index_number != oindex) {
				if (!temp->next_item) {
					throw runtime_error("Book::reindex: cannot find original index in a certain token");
				}
				temp = temp->next_item;
			}
			temp->index_number = index;
		}else {
			throw runtime_error("Book::reindex: token not found!");
		}
	}
	return original.traceback;
}

void Book::add(istream& ist) {
	string start;
	int index;
	while(!ist.eof()) {
		ist >> start;
		istringstream convert(start);
		if (convert >> index) {
			char id;
			string bookname;
			ist >> id;
			char c;
			ist.get(c);
			while(c==' ') {
				ist.get(c);
			}
			ist.putback(c);
			getline(ist, bookname);
			switch (id) {
			case 't':
			{
				bool success = add(index, bookname);
				if (!success) {
					throw runtime_error("Book::add: fixed index exists!");
				}
				break;
			}
			case 'f':
			{
				bool success = add(bookname);
				if (!success) {
					throw runtime_error("Book::add: bookname exists!");
				}
				break;
			}
			}
//			ist.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			string following;
			getline(ist, following);
			start += following;
			bool success = add(start);
			if(!success) {
				throw runtime_error("Book::add: bookname exists!");
			}
		}
	}
}


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
		booklist.head->volume_next = nullptr;
		{vector<string> tokens = get_tokens(name);
		int size = tokens.size();
		for (int i = 0; i < size; i++) {
			this->index.add_token(tokens[i])->add(0);
		}
		}
		return true;
	case 'X':
	{
//		if(!booklist.head) {
//			volume* add = new volume;
//			add->volume_next = booklist.head;
//			add->index_number = 0;
//			add->name = name;
//			add->fixed_index = false;
//			booklist.head = add;
//		}

		if (booklist.head->index_number > 0) {
			volume* add = new volume;
			add->volume_next = booklist.head;
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
		int index = 0;
		volume* xconti = booklist.head;
		volume* track = nullptr;
		while (xconti->index_number == index&&xconti->volume_next) {
			track = xconti;
			xconti = xconti->volume_next;
			index++;
		}
		if(track==nullptr) {
			booklist.head = new volume;
			booklist.head->name = name;
			booklist.head->fixed_index = false;
			booklist.head->index_number = 0;
			booklist.head->volume_next = nullptr;

		}
		else {
			booklist.add(track, index, name, false);
		}
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
		booklist.head->volume_next = nullptr;
		}
		return true;
	case 'S':
		{
		volume* newh = new volume;
		newh->name = name;
		newh->index_number = index;
		newh->fixed_index = true;
		newh->volume_next = booklist.head->volume_next;
		booklist.head = newh;
		return true;
		}
	case 'L': 
	case 'M':
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
		if (temp.traceback) {
			temp.traceback = temp.rtn->volume_next;
			temp.rtn->volume_next = nullptr;
			delete temp.rtn;
		}
		else {
			delete temp.rtn;
			booklist.head = nullptr;
		}
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
			temp.traceback = temp.rtn->volume_next;
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
	if(!temp->head) {
		return true;
	}
	if(temp->head->index_number==-1) {
		return false;
	}
	return true;
}

void Book::load(){
	index_input.open("index.dat");
	if(!index_input) {
		return;
	}
	index.load(index_input);
	index_input.close();
	booklist_input.open("booklist.dat");
	if(!booklist_input) {
		return;
	}
	istream& ist = booklist_input;
	add(ist);
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
	if(!booklist_output) {
		throw runtime_error("Book::save: cannot open 'booklist.dat' for writing!");
	}
	if(booklist.head) {
		volume* current = booklist.head;
		while(true) {
			booklist_output << current->index_number << ' ';
			if(current->fixed_index) {
				booklist_output << "t ";
			}else {
				booklist_output << "f ";
			}
			booklist_output << current->name;
			if(!current->volume_next) {
				break;
			}
			else {
				booklist_output << '\n';
			}
			current = current->volume_next;
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
		} while (book->next_item);
		cout << book->index_number;
		cout << "\0}\0";
	}
	for(unsigned char i=0; i<CharNum; i++) {
		if(current->next_item[i]) {
			cout << reinterpret_cast<char&>(i);
			save_loop(ofs, current->next_item[i]);
		}
	}
}*/



