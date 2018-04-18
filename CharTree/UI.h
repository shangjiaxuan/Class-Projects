#pragma once

#include "Booklist.h"

class UI {
public:
	void UI_main();
//private:
	Book base;

	void add();
	bool add_file(std::string cmd);
	void add_manual(std::string cmd);
	void add_stream(std::istream& ist);
	//base function
	void add_string(std::string line);

	void del();
	void del_manual();
	void del_file();
	void del_stream(std::istream& ist);
	//base function
	void del_string(std::string line);

	void list();
	void list_book();
	void list_index();
	void list_stream(std::ostream& ost);
	//base function
	void list_string(std::string line);

	std::string parse_bookname(std::string line);
	std::string parse_path(std::string line);
};

