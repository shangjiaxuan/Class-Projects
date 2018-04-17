#pragma once

#include "Header.h"
#include "CharTree.h"


//所有书目中一本书对应的链表节点
struct volume {
	volume() {
		index_number = -2;
		next = nullptr;
	}
	//利用delete递归调用析构函数的特点一次性释放书目链表
	~volume() {
		if (next) {
			next->~volume();
			delete next;
			next = nullptr;
		}
	}
	int index_number;
	bool fixed_index{ false };
	std::string name;
	volume* next;
};

//管理所有书目的链表
//貌似应该再加一个保存到文件的功能，就叫“index.dat”和“booklist.dat”罢
//这样就不需要非token的vector了（手动输入一次，保存）
//最好加上取消非token的功能
//但这样就要写一个遍历的函数，查找所有含有这个新token的书目，添加
//最好把书和next指针分开封装，这样可以让chartree的节点单链表也含有书名信息，方便访问（不查找，不一定要用find(index).name）
class Book {
public:
	Book() {
//		CharTree index;
//		volume booklist_head;
		booklist_head = nullptr;
		init_ntoken();
		load();
	}
	~Book() {
//		delete &booklist_head;
//		delete &index;
	};
	//the linked list of books
	volume* booklist_head;
	//the index_number tree of tokens
	CharTree index;
	//if false, the index_number is already assigned as fixed
	//the current book using the index_number is not assigned as fixed, will return true
	//and call the corresponding functions to fix the linked list in the index_number
	//(what really matters)
	bool add(int index, std::string name);
	//if false, the name's already taken.
	bool add(std::string name);
	bool del(int index);
	bool del(std::string name);
	void save();
	void load();
private:
	ifstream index_input;
	ofstream index_output;
	ifstream booklist_input;
	ofstream booklist_output;
	//用于find函数的返回值
	struct found {
		//the status of the found item
		//legend:
		//N: booklist_head==nullptr
		//L: the specified index_number is larger than all current ones, rtn is the tail pointer
		//S: the specified index_number is smaller than all current ones, rtn is the booklist_head pointer
		//M: the specified index_number is in the middle of current ones, rtn is the largest one smaller than the specified index_number
		//P: the specified index_number is found at rtn;
		//X: book name string not found
		char status;
		volume* rtn;
		volume* traceback;
		std::string name;
	};
	found find(int index);
	found find(std::string name);
//这个函数不安全，放在private里面
	void add(volume* previous, int index, std::string name, bool index_type);
//reassigns a default index_number and returns the volume before the index_number
	volume* reindex(found original);
	void To_standard(std::string& bookname);
	//为了加速，让非token对应的chartree节点的index头字符串对应一本编号
	//“-1”的书
	bool istoken(std::string token);
//vector 本身访问速度很慢，可以考虑其他方法，比如char**，但那样又要动态分配很麻烦
	std::vector<std::string> get_tokens(std::string bookname);
	void init_ntoken();
//先简单地写一个表，肯定不完整，以后用户可以调用函数添加（同时删除原有链表）
	std::vector<std::string> default_non_tokens{"THE","AM","IS","ARE","OF","AT","TO","UNDER","ABOVE"};
};
