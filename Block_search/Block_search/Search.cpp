﻿#include "Search.h"

using namespace std;

bool on{ true };

search::search() {
	ifstream ifs;
	string cmd;
	cout << "Please input the sorted array of numbers you wish to search from:\n";
//可以考虑可以手动输入
	getline(cin,cmd);
	cmd = parse_path(cmd);
	ifs.open(cmd);
	if(!ifs) {
		throw runtime_error("search::search(): cannot open specified file!");
	}
	string block;
	size_t line{0};
	size_t array{0};
	int temp;
	while(!ifs.eof()) {
		getline(ifs, block);
		if (block == "") {
			break;
		}
		line++;
		istringstream iss(block);
		while(iss>>temp) {
			array++;
		}
	}
	ifs.close();
	ifs.open(cmd);
	if (!ifs) {
		throw runtime_error("search::search(): cannot reopen specified file!");
	}
	data = new int[array];
	index = new index_block[line];
	block_num = line;
	line = 0;
	array = 0;
	while (!ifs.eof()) {
		getline(ifs, block);
		if (block == "") {
			break;
		}
		index[line].data = data;
		index[line].block_size = 0;
		index[line].start = array;
		istringstream iss(block);
		while (iss >> temp) {
//			index[line][index[line].block_size] = temp;
			data[array] = temp;
			index[line].block_size++;
			if(temp>index[line].max){
				index[line].max=temp;
			}
			array++;
		}
		line++;
	}
	cout << endl;
}

search::~search() {
	delete[] data;
	data = nullptr;
	delete[] index;
	index = nullptr;
}

void search::print(ostream& ost) {
	for(int i=0; i<block_num; i++){
		for(int j=0; j<index[i].block_size; j++) {
			ost<<index[i][j]<<'\t';
		}
		ost<<'\n';
	}
	ost<<endl;
}

size_t search::search_index(int code) {
	size_t low=0;
	size_t high=block_num-1;
	size_t mid;
	while(true){
		mid=(low+high)/2;
		if(low==high){
			return mid;
		}
		else if(index[mid].max<code){
			low=mid+1;
		}
		else if(index[mid].max>code){
			high=mid;
		}
	}
};

long long search::search_block(size_t block, int code){
	for(size_t i=0; i<index[block].block_size; i++){
		if(index[block][i]==code){
			return i;
		}
	}
	return -1;
}

search::coordinate search::search_code(int code){
	coordinate rtn;
	rtn.block=search_index(code);
	rtn.place=search_block(rtn.block,code);
	return rtn;
}

void search::print_result(ostream& ost, const coordinate& cor){
	if(cor.place<0){
		ost<<"The code is not found!\n";
	}
	else{
		ost << "The code is found in place number " << cor.block+1 << " in line " << cor.block+1 << " .\n";
	}
	return;
}

void search::search_cmd() {
	int code;
	cin >> code;
	coordinate cor = search_code(code);
	print_result(cout, cor);
}

void search::UI() {
	string cmd;
	cout << "Please input a command:\n";
	cin >> cmd;
	if (cmd == "search") {
		cout << endl;
		search_cmd();
	}
	else if (cmd == "retarget") {
		conti = false;
		char c;
		do{
			cin.get(c);
		} while (c != '\n');
	}
	else if (cmd == "exit") {
		on = false;
		conti = false;
	}
	else {
		throw runtime_error("search::UI(): Unkown command!");
	}
	cout << endl;
}


void search::name_ver() {
	cout << "Simple index search program\n";
	cout << "Version 0.0.0.0\n";
	cout << endl;
}

