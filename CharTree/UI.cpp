#include "UI.h"

using namespace std;

void UI::UI_main() {
	string cmd;
	cout << "Please enter a command:" << endl;
	cin >> cmd;
	if(cmd=="add") {
		add();
	}
	else if(cmd=="del") {
		del();
	}
	else if(cmd=="list") {
		list();
	}
	else if(cmd=="exit") {
		on = false;
	}
}

void UI::add() {
	string cmd;
	getline(cin, cmd);
	if(add_file(cmd)) {
		return;
	}
	add_manual(cmd);
}

bool UI::add_file(string cmd) {
	ifstream ifs;
	string path = parse_path(cmd);
	ifs.open(path);
	if(ifs) {
		add_stream(ifs);
		ifs.close();
		return true;
	}
	return false;
}

void UI::add_manual(string cmd) {
	cout << "Adding book " << cmd << endl;
	add_string(cmd);
}

void UI::add_stream(istream& ist) {
	string cmd;
	while(!ist.eof()) {
		getline(ist, cmd);
		if(cmd!="") {
			add_string(cmd);
		}
	}
}

void UI::add_string(std::string line) {
	//有序号的可能
	istringstream iss(line);
	int index; string name;
	if(iss>>index) {
		getline(iss, name);
		name = parse_bookname(name);
		base.add(index, name);
		return;
	}
	name = parse_bookname(line);
	base.add(name);
}

void UI::del() {
	
}

void UI::del_file() {
	
}

void UI::del_manual() {
	
}

void UI::del_stream(std::istream& ist) {
	
}

void UI::del_string(std::string line) {
	
}

void UI::list() {
	
}

void UI::list_book() {
	
}

void UI::list_index() {
	
}

void UI::list_stream(std::ostream& ost) {
	
}

void UI::list_string(std::string line) {
	
}

string UI::parse_bookname(string line) {
	istringstream iss(line);
	string current;
	string name("");
	while (iss) {
		current = "";
		iss >> current;
		name.append(current);
		name.push_back(' ');
	}
	name.pop_back();
	return name;
}

string UI::parse_path(string line) {
	string path;
	istringstream iss(line);
	char current;
	iss.get(current);
	while (current == ' ') {
		iss.get(current);
	}
	iss.putback(current);
	while (iss) {
		iss.get(current);
		switch (current) {
		case '\"':
			iss.get(current);
			while (current != '\"') {
				path.push_back(current);
				iss.get(current);
			}
			break;
		default:
			path.push_back(current);
			iss.get(current);
		}
	}
	return path;
}
