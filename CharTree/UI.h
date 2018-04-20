#pragma once

#include "Booklist.h"

class UI {
public:
	void UI_main();
//private:
	bool on;
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
	void list_book_stream(std::ostream& ost);
	void list_index();
	void list_index_stream(std::ostream& ost);

	//adds a string that is not considered to be a token
	void ntoken(std::string);

	std::string parse_bookname(std::string line);
	std::string parse_path(std::string line);

	void ignore_space(std::istream& ist);
};

