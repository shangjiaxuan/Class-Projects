#define CROSSING

#include "Crossing.h"

namespace Crossing {
	Roads::Roads() {
		std::cout << "Please input the number of roads at the junction:\n" << std::endl;
		std::cin >> number;
		in = new bool[number];
		out = new bool[number];
		std::cout << "\nPlease input the parameters for each road. '-1' for one-way in, '1' for one-way out, and '0' for two-way road.\n";
		std::cout << "Note: Input in a clockwise orderly manner\n" << std::endl;
		try {
			this->get_status();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		possi = no_in * no_out - two_ways;
		this->routes = new route[possi];
		this->init(this->routes);
		return;
	}

/*	Roads::Roads(short num) {
		number = num;
		in = new bool[number];
		out = new bool[number];
		std::cout << "\nPlease input the parameters for each road. '-1' for one-way in, '1' for one-way out, and '0' for two-way road.\n";
		std::cout << "Note: Input in a clockwise orderly manner\n" << std::endl;
		std::cout << "Note: Input in a clockwise orderly manner\n" << std::endl;
		try {
			this->get_status();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		possi = no_in * no_out - two_ways;
		this->routes = new route[possi];
		this->init(this->routes);
		return;
	}
*/

	void Roads::get_status() {
		two_ways = 0;
		no_in = 0;
		no_out = 0;
		short s;
		for (int i = 0; i < number; i++) {
			std::cin >> s;
			switch (s) {
			case -1:
				in[i] = true;
				out[i] = false;
				no_in++;
				break;
			case 0:
				in[i] = true;
				out[i] = true;
				no_in++;
				no_out++;
				two_ways++;
				break;
			case 1:
				in[i] = false;
				out[i] = true;
				no_out++;
				break;
			default:
				throw std::runtime_error("get_status: Unkown specifier!");
			}
		}
	}

	void Roads::init(route*& rt) {								//初始化路径和参数的对应表
		int i = 0; int cur_offset = 0;
		while (i < number) {
			if (in[i]) {
				int j = 0;
				while (j < number) {							//调头的不算
					if (out[j] && j != i) {
						rt[cur_offset].from = i;
						rt[cur_offset].to = j;
						rt[cur_offset].offset = cur_offset;		//不想再让它计算了，存储量不多，调用应该蛮多的
						cur_offset++;
					}
					j++;
				}
			}
			i++;
		}
	}

/*	relations::relations() {
		std::cout << "Please input the number of roads at the junction:\n" << std::endl;
		std::cin >> number;
		in = new bool[number];
		out = new bool[number];
		std::cout << "\nPlease input the parameters for each road. '-1' for one-way in, '1' for one-way out, and '0' for two-way road.\n";
		std::cout << "Note: Input in a clockwise orderly manner\n" << std::endl;
		std::cout << "Note: Input in a clockwise orderly manner\n" << std::endl;
		try {
			this->get_status();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		possi = no_in * no_out - two_ways;
		this->routes = new route[possi];
		this->init(this->routes);
		this->data = new bool[possi*possi];
		this->map = new bool*[possi];
		for (int i = 0; i < possi; i++) {
			this->map[i] = this->data + i * possi * sizeof(bool);
		}
		return;
	}
*/

	short relations::left_dist(short s1, short s2) {				//两个路口，s1来看s2在它左面几个
		short temp = s2 - s1;									//约束了算法中顺时针编号
		if (temp < 0) { temp += number; }						//姑且认为没有错误使用，s1!=s2
//		std::cout << "Left dist : The distance between " << s1 << " and " << s2 << " is " << temp << std::endl;
		return temp;
	}

	short relations::right_dist(short s1, short s2) {				//s1来看s2在它右面几个
		short temp = s1 - s2;									//约束了算法中顺时针编号
//		std::cout << "Right dist : The distance between " << s1 << " and " << s2 << " is " << temp << std::endl;
		if (temp < 0) { temp += number; };
		return temp;
	}
	
	char relations::rela(Roads::route rt, short pos) {										//看一个路口在一条路左面还是右面。a为左，b为右，c为与到达点重合，d为与出发点重合
		if (left_dist(rt.from, pos) < left_dist(rt.from, rt.to)) { return 'a'; }
		if (right_dist(rt.from, pos) < right_dist(rt.from, rt.to)) { return 'b'; }
		if (pos == rt.to) { return 'c'; }
		if (pos == rt.from) { return 'd'; }
		throw std::runtime_error("rela: Undable to find relative position");
	}

	bool relations::compatible(Roads::route& rt1, Roads::route& rt2) {								//注意靠右行驶的问题
		if (rt1.from == rt2.from&&rt1.to == rt2.to) {							//在创建表的时候有这一项。。。
//			std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are not compatible because they are the same" << std::endl;
			return false;														//自己不能也计入“compatible”的行列，比较特别
		}
		if (rela(rt1, rt2.from) == rela(rt1, rt2.to)) {							//先考虑一般情况，并且认为已经处理好了掉头
//			std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are compatible because they have no intersections" << std::endl;
			return true;														//剩下的允许的必须存在路口重合的情况
		}																		//重合有四种情况
		if (rt1.from == rt2.from || rt1.to == rt2.to) {							//考虑同一个路口出入车
//			std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are compatible because they come from or go into the same road" << std::endl;
			return true;														//不管怎么样都不会撞
		}
		if (rt1.to == rt2.from) {												//考虑入口与出口重合，要考虑靠右行
			if (left_dist(rt1.from, rt1.to) >= right_dist(rt2.from, rt2.to)) {
//				std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are compatible because the cars are on the right." << std::endl;;
				return true;
			}
//			std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are not compatible because the cars are on the right." << std::endl;;
			return false;
		}
		if (rt2.to == rt1.from) {
			if (left_dist(rt2.from, rt2.to) >= right_dist(rt1.from, rt1.to)) {
//				std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are compatible because the cars are on the right." << std::endl;;
				return true;
			}
//			std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are not compatible because the cars are on the right." << std::endl;;
			return false;
		}
//		std::cout << rt1.from << "->" << rt1.to << " and " << rt1.from << "->" << rt1.to << " are not compatible because they intersect." << std::endl;;
		return false;
	}

	/////////////////////////////////////////////////////////////////////
	//这是用来生成老师程序里对应的输入关系表的函数，但原则上完全可以每调用一次就
	//算一次compatible(rt1,rt2)，主要区别是内存和计算量的权衡，可以考虑都试一下，
	//看看哪个比较好
	void relations::find_relations() {
		std::cout << "\nGenerating relation map...\n";
		std::cout << "Please wait...\n" << std::endl;;
		for (int i = 0; i < possi; i++) {
//			std::cout << "i is " << i << " now" << std::endl;
			for (int j = 0; j < possi; j++) {
//				std::cout << "j is " << j << " now" << std::endl;
				this->map[i][j] = compatible(this->This->routes[i], this->This->routes[j]);
			}
		}
	}
	void relations::print_rel() {
		std::cout << "The following is the found relationship map between the routes:\n" << std::endl;
		for (int i = 0; i<possi; i++) {
			std::cout << this->This->routes[i].from << "->" << this->This->routes[i].to << ": " << '\t';
			for (int j = 0; j<possi; j++) {
				std::cout << this->map[i][j] << '\t';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}
