#include "Booklist.h"

using namespace std;

list::found list::find(int index) {
	static int default_index;
	int cur_index{ 0 };
	if (!head) {
		return { 'N', nullptr , nullptr , "" , 0 };
	}
	if (index < head->index_number) {
		return { 'S' , nullptr, head->volume_next, "", 0 };
	}
	if (index == head->index_number) {
		volume* temp = head;
		while (temp->volume_next != nullptr) {
			if (temp->index_number == cur_index) {
				cur_index++;
			}
			else {
				default_index = cur_index;
			}
			temp = temp->volume_next;
		}
		
		return { 'P' , head, nullptr, head->name, index };
	}
	volume* current = head;
	volume* traceback = nullptr;
	while (index > current->index_number) {
		if(cur_index==current->index_number) {
			cur_index++;
		}else {
			default_index = cur_index;
		}
		if (current->volume_next == nullptr) {
			default_index = cur_index + 1;
			return { 'M' , current, nullptr, "", default_index };
		}
		traceback = current;
		current = current->volume_next;
	}
	if (index == current->index_number) {
		volume* temp = head;
		while (temp->volume_next != nullptr) {
			if (temp->index_number == cur_index) {
				cur_index++;
			}
			else {
				default_index = cur_index;
			}
			temp = temp->volume_next;
		}
		default_index = cur_index + 1;
		return { 'P' , current, traceback, current->name, default_index };
	}
	volume* temp = head;
	while (temp->volume_next != nullptr) {
		if (temp->index_number == cur_index) {
			cur_index++;
		}
		else {
			default_index = cur_index;
		}
		temp = temp->volume_next;
	}
	default_index = cur_index + 1;
	return { 'L' , nullptr, traceback->volume_next, "", default_index };
}

list::found list::find(std::string name) {
	if (!head) {
		return { 'N', nullptr , nullptr, "", 0 };
	}
	static int index;
	int cur_index = 0;
	volume* current = head;
	volume* traceback = nullptr;
	if (head->name != name) {
		if(head->index_number>0) {
			index = 0;
		}
		while (current->volume_next != nullptr) {
			if (current->index_number == cur_index) {
				cur_index++;
			}
			else {
				index = cur_index;
			}
			traceback = current;
			current = current->volume_next;
			if (current->name == name) {
				break;
			}
		}
	}
	volume* temp = current;
	while (temp->volume_next != nullptr) {
		if (temp->index_number == cur_index) {
			cur_index++;
		}
		else {
			index = cur_index;
		}
		temp = temp->volume_next;
	}
	index = cur_index + 1;
	if (current->name == name) {
		return { 'P' , current, traceback, name, index };
	}
	return { 'X' , nullptr , nullptr, name, index };
}


int list::add(std::string name) {
	return add(-1, name);
}

int list::add(int index, std::string name) {
	found found;
	bool fixed;
	if (index<0) {
		fixed = false;
		found = find(name);
		index = found.default_index;
	}
	else {
		fixed = true;
		found = find(index);
	}
	switch(found.status) {
	case 'P':
	{
		if (found.rtn->fixed_index) {
			throw runtime_error("list::add: the specified index (or book name if no index specified) exists!");
		}
		volume* added = new volume;
		added->name = name;
		added->index_number = index;
		added->fixed_index = fixed;
		reindex(found, added);
		break;
	}
	case 'N':
		add_head(index, name, fixed);
		break;
	case 'S':
		change_head(index, name, fixed);
		break;
	case 'L': case 'M':
		add(found.rtn, index, name, fixed);
		break;
	default:
		throw runtime_error("list::add: unknown status!");
	}
	return index;
}


//void list::add_head(int index, std::string name) {
//	add_head(index, name, true);
//}

//void list::add_head(std::string name) {
//	add_head(default_index(), name, false);
//}

void list::add_head(int index, std::string name, bool fixed) {
	head = new volume;
	head->index_number = index;
	head->name = name;
	head->fixed_index = fixed;
	head->volume_next = nullptr;
}

//void list::change_head(int index, std::string name) {
//	change_head(index, name, true);
//}
//
//void list::change_head(std::string name) {
//	change_head(default_index(), name, false);
//}

void list::change_head(int index, std::string name, bool fixed) {
	volume* temp = head;
	head = new volume;
	head->name = name;
	head->index_number = index;
	head->volume_next = temp;
	head->fixed_index = fixed;
}

void list::add(volume* previous, int index, std::string name, bool index_type) {
	volume* current = new volume;
	current->index_number = index;
	current->fixed_index = index_type;
	current->name = name;
	current->volume_next = previous->volume_next;
	previous->volume_next = current;
}

/*
int list::default_index() {
	volume* current = head;
	int index = 0;
	while(current) {
		if(current->index_number>index) {
			return index;
		}
		current = current->volume_next;
		index++;
	}
	return index;
}
*/

volume* list::find_default() {
	volume* traceback = nullptr;
	volume* current = head;
	int index = 0;
	while (current) {
		if (current->index_number>index) {
			return traceback;
		}
		current = current->volume_next;
		index++;
	}
	return traceback;
}

int list::reindex(found original, volume* added) {
	if(original.status!='P') {
		throw runtime_error("list::reindex(found): original book not found!");
	}
	original.traceback->volume_next = added;
	added->volume_next = original.rtn->volume_next;
	volume* default_location = find_default();
	if(!default_location) {
		throw runtime_error("list::reindex: list cannot be empty!");
	}
	original.rtn->volume_next = default_location->volume_next;
	default_location->volume_next = original.rtn;
	original.rtn->index_number = default_location->index_number + 1;
	return original.rtn->index_number;
}

int list::del(string name) {
	volume* current = head;
	volume* traceback = nullptr;
	int index;
	while(current) {
		if(current->name==name) {
			index = current->index_number;
			if(!traceback) {
				delete head;
				head = nullptr;
			}
			else {
				traceback->volume_next = current->volume_next;
				current->volume_next = nullptr;
				delete current;
				current = nullptr;
			}
			return index;
		}
		if(!current->volume_next) {
			throw runtime_error("list::del: bookname not found!");
		}
		current = current->volume_next;
	}
	throw runtime_error("list::del list is empty!");
}

std::string list::del(int index) {
	volume* current = head;
	volume* traceback = nullptr;
	string name;
	while (current) {
		if (current->index_number == index) {
			name = current->name;
			if (!traceback) {
				delete head;
				head = nullptr;
			}
			else {
				traceback->volume_next = current->volume_next;
				current->volume_next = nullptr;
				delete current;
				current = nullptr;
			}
			return name;
		}
		if (current->index_number>index || !current->volume_next) {
			throw runtime_error("list::del: index not found!");
		}
		current = current->volume_next;
	}
	throw runtime_error("list::del list is empty!");
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

void Book::reindex(list::found original, volume* new_book) {
	int new_index = booklist.reindex(original, new_book);
	if (original.name == "") {
		throw runtime_error("Book::reindex: Empty bookname!");
	}
	vector<string> tokens = get_tokens(original.name);
	int size = tokens.size();
	for(int i=0; i<size; i++) {
		node* token_loc = index.locate(tokens[i]);
		token_loc->add(new_index);
	}
}

/*
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
*/

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
				add(index, bookname);
				break;
			case 'f':
				add(bookname);
				break;
			default:
				throw runtime_error("Book::add: unknown fixed status!");
			}
//			ist.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			string following;
			getline(ist, following);
			start += following;
			add(start);
		}
	}
}

void Book::add(string name) {
	int index = booklist.add(name);
	add_book_tree(name, index);
}

void Book::add(int index, std::string name) {
	booklist.add(index, name);
	add_book_tree(name, index);
}

/*bool Book::add(std::string name) {
	To_standard(name);
	list::found temp = booklist.find(name);
	switch (temp.status) {
	case 'P':
		return false;
	case 'N':
		booklist.add_head(name);
		add_book_tree(name, 0);
		return true;
	case 'X':
	{
		if (booklist.head->index_number > 0) {
			booklist.change_head(0, name);
			add_book_tree(name, 0);
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
			booklist.add_head(name);
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
}*/

/*bool Book::add(int index, std::string name) {
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
}*/

bool Book::del(int deleted_index) {
	try {
		string name = booklist.del(deleted_index);
		del_book_tree(name, deleted_index);
	}
	catch (exception& e) {
		cerr << e.what();
		return false;
	}
	return true;
/*	list::found temp = booklist.find(deleted_index);
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
			index.del_token(tokens[i]);
		}
		}
		return true;
	case 'N': case 'L': case 'M': case 'S':
		return false;
	default:
		throw runtime_error("del(int): Unknown status!");
	}
	*/
}

bool Book::del(std::string name) {
	To_standard(name);
	try {
		int index = del(name);
		del_book_tree(name, index);
	}
	catch(exception& e) {
		cerr << e.what();
		return  false;
	}
	return true;
/*
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
			index.del_token(tokens[i]);
		}
		}
		temp.rtn = nullptr;
		return true;
	case 'N': case 'X':
		return false;
	default:
		throw runtime_error("del(string): Unknown status!");
	}
*/
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

void Book::add_book_tree(std::string bookname, int book_index) {
	vector<string> tokens = get_tokens(bookname);
	int size = tokens.size();
	for (int i = 0; i<size; i++) {
		index.add_token(tokens[i])->add(book_index);
	}
}

void Book::del_book_tree(std::string bookname, int book_index) {
	vector<string> tokens = get_tokens(bookname);
	int size = tokens.size();
	for (int i = 0; i<size; i++) {
		node* node = index.locate(tokens[i]);
		item* list = node->head;
		if(!list) {
			index.del_token(tokens[i]);
			continue;
		}
		if(!list->next_item&&list->index_number==book_index) {
			index.del_token(tokens[i]);
			continue;
		}
		node->del(book_index);
	}
}


