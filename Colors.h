#pragma once

#include "Crossing.h"

class Colors {
public:
	Colors() {
		rel = nullptr;
		group = nullptr;
		throw std::runtime_error("UpColors: Please specify a relation to use this class!");
	}
	Colors(Crossing::relations& rela) {
		rel = &rela;
		group = new int[Crossing::possi];
	}
	~Colors() {
		delete[] this->group;
	}
	void find_groups();
	void print_result();
protected:
	Crossing::relations* rel;
	int color_num{ 0 };
	int* group;									//和struct route对应的分组数组
	bool evaluate(int cur, int opp);			//看某个元素能否加入目前的组
	void assign(int add);						//给某个元素分配一个组
};
