#include "Cars.h"

using namespace std;

namespace vehicle_control {

	void controls::interaction() {
		while (true) {
			string cmd;
			cout << "Please enter a command..." << endl;
			cin >> cmd;
			if (cmd == "exit") {
				exit(0);
			}
			if (cmd=="save") {
				save();
			}
			if (cmd=="load") {
				load();
			}
			if (cmd == "add") {
				master_add();
			}
			else if (cmd == "remove") {
				master_remove();
			}
			else if (cmd == "find") {
				master_find();
			}
			else if (cmd == "list") {
				list();
			}
		}
	}

	void controls::list() {
		for (int i = 0; i < car_num; i++) {
			cout << "Detailed information on the car is as follows:\n";
			cout << "id:\t" << all_cars[i].id << '\n';
			cout << "type:\t" << all_cars[i].type << '\n';
			cout << "price:\t" << all_cars[i].price << '\n';
			cout << "driver:\t" << all_cars[i].driver << '\n' << endl;
			if (!all_cars[i].violations) { break; }
			cout << "violations:\t" << endl;
			violation* current = all_cars[i].violations;
			if (current) {
				do {
					cout << "code:\t\t" << current->code << '\n';
					cout << "time:\t\t" << current->time << '\n';
					cout << "location:\t" << current->location << '\n';
					if (current->dealt) {
						cout << "The incident has been dealt with.\n" << endl;
					}
					else {
						cout << "The incident has not been not dealt with yet.\n" << endl;
					}
					if (current->next) {
						current = current->next;
					}
				} while (current->next);
			}
			cout << endl;
		}
	}

	void controls::save() {
		ofstream ofs;
		ofs.open("save.dat");
		for (int i = 0; i < car_num; i++) {
			ofs << all_cars[i].id << ' ';
			ofs << all_cars[i].type << ' ';
			ofs << all_cars[i].price << ' ';
			ofs << all_cars[i].driver << ' ';
			if (!all_cars[i].violations) {
				ofs << '\n';
				break;
			}
			ofs << '{';
			violation* current = all_cars[i].violations;
			if (current) {
				do {
					ofs << current->code << ' ';
					ofs << current->time << ' ';
					ofs << current->location << ' ';
					if (current->dealt) {
						ofs << "t\t";
					}
					else {
						ofs << "f\t";
					}
					if (current->next) {
						current = current->next;
					}
				} while (current->next);
			}
			ofs << "}\n";
		}
		ofs.close();
	}

	void controls::load() {
		ifstream ifs;
		ifs.open("save.dat");
		if (!ifs) {
			ofstream ofs;
			ofs.open("save.dat");
			ofs.close();
			ifs.open("save.dat");
			if(!ifs) {
				throw runtime_error("controls::load(): Cannot open file for loading!");
			}
			ifs.close();
			return;
		}
		for(int i=0; i<MAX_CAR; i++) {
			if(ifs.eof()){break;}
			car_num++;
			int id; string type; double price; string driver;
			ifs >> id >> type >> price >> driver;
			all_cars[i].id = id;
			all_cars[i].type = type;
			all_cars[i].price = price;
			all_cars[i].driver = driver;
			while(ifs.peek()==' ') {
				ifs.get();
			}
			if(ifs.peek()!='{') {
				all_cars[i].violations = nullptr;
				while(!isdigit(ifs.peek())&&!ifs.eof()) {
					ifs.get();
				}
				continue;
			}
			all_cars[i].violations = new violation;
			violation* current = all_cars[i].violations;
			ifs.get();
			while(ifs.peek()!='}') {
				int code; int time; string location; char dealt;
				ifs >> code >> time >> location >> dealt;
				current->code = code;
				current->time = time;
				current->location = location;
				switch(dealt) {
				case 't':
					current->dealt = true;
					break;
				case 'f':
					current->dealt = false;
					break;
				default:
					throw runtime_error("load: unknown 'dealt' status!");
				}
				current->next = nullptr;
				ifs.get();
				char c = ifs.peek();
				if(ifs.peek()!='}') {
					current->next = new violation;
					current = current->next;
				}
			}
			ifs.ignore(3, '\n');
		}
	}

	void controls::master_add() {
		string sw;
		cin >> sw;
		if (sw == "car") {
			add_car();
		}
		else if (sw == "violation") {
			add_violation();
		}
		else {
			throw runtime_error("master_add: unknown command");
		}
	}

	void controls::master_remove() {
		string sw;
		cin >> sw;
		if (sw == "car") {
			int id;
			cin >> id;
			remove_car(id);
		}
		else if (sw == "violation") {
			int code;
			cin >> code;
			remove_violation(code);
		}
		else {
			throw runtime_error("master_remove: unknown command");
		}
	}

	void controls::master_find() {
		string sw;
		cin >> sw;
		if (sw == "car") {
			int id;
			cin >> id;
			find_car_id(id);
		}
		else if (sw == "violation") {
			find_violation();
		}
		else {
			throw runtime_error("master_find: unknown command");
		}
	}

	void controls::add_car() {
		string s;
		getline(cin, s);
		if (manual_add_car(s)) {
			cout << "\nSuccess!\n" << endl;
			return;
		}
		if (file_add_car(s)) {
			cout << "\nSuccess!\n" << endl;
			return;
		}
		throw runtime_error("add_car: cannot identify command!");
	}

	bool controls::file_add_car(std::string cmd) {
		istringstream iss(cmd);
		string parsed;
		while (iss.peek() == ' ') {
			iss.get();
		}
		getline(iss, parsed);
		ifstream ifs;
		ifs.open(parsed);
		if (!ifs) {
			return false;
		}
		string entry;
		while (!ifs.eof()) {
			getline(ifs, entry);
			istringstream iss_entry(entry);
			int id; string type; double price; string driver;
			if (!(iss_entry >> id >> type >> price >> driver)) {
				throw runtime_error("file_add_car: wrong input format in file");
			}
			car temp{ id,type,price,driver };
			all_cars[car_num] = temp;
			car_num++;
		}
		ifs.close();
		return true;
	}

	bool controls::manual_add_car(std::string cmd) {
		istringstream iss(cmd);
		int id; string type; double price; string driver;
		if (!(iss >> id >> type >> price >> driver)) {
			return false;
		}
		car temp{ id,type,price,driver };
		all_cars[car_num] = temp;
		car_num++;
		return true;
	}

	void controls::add_violation() {
		string s;
		getline(cin, s);
		if (manual_add_violation(s)) {
			cout << "\nSuccess!\n" << endl;
			return;
		}
		if (file_add_violation(s)) {
			cout << "\nSuccess!\n" << endl;
			return;
		}
		throw runtime_error("add_violation: cannot identify command!");
	}

	bool controls::file_add_violation(std::string cmd) {
		istringstream iss(cmd);
		string parsed;
		while (iss.peek() == ' ') {
			iss.get();
		}
		getline(iss, parsed);
		ifstream ifs;
		string entry;
		ifs.open(parsed);
		if (!ifs) {
			return false;
		}
		while (!ifs.eof()) {
			getline(ifs, entry);
			if (manual_add_violation(entry)) {}
			else {
				throw runtime_error("file_add_violation: wrong entry format!");
			}
		}
		return true;
	}

	bool controls::manual_add_violation(std::string cmd) {
		int code; string location; int time; bool dealt; string dealt_i; vector<int> car_place;
		istringstream iss(cmd);
		if (!(iss >> code >> location >> time >> dealt_i)) {
			return false;
		}
		int id;
		while (iss) {
			if (iss >> id) {
				car_place.push_back(core_find_index_in_all_cars(id));
				iss.get();
			}
			else {
				throw runtime_error("manual_add_violation: wrong id!");
			}
		}
		int size = car_place.size();
		if (dealt_i == "y" || dealt_i == "Y" || dealt_i == "true") {
			dealt = true;
		}
		else {
			dealt = false;
		}
		for (int i = 0; i < size; i++) {
			int place = car_place[i];
			if (!all_cars[place].violations) {
				all_cars[place].violations = new violation;
				all_cars[place].violations->location = location;
				all_cars[place].violations->code = code;
				all_cars[place].violations->dealt = dealt;
				all_cars[place].violations->next = nullptr;
				all_cars[place].violations->time = time;
				break;
			}
			violation* temp = new violation;
			temp->location = location;
			temp->code = code;
			temp->dealt = dealt;
			temp->time = time;
			temp->next = all_cars[place].violations;
			all_cars[place].violations = temp;
		}
		return true;
	}

	void controls::remove_car(int id) {
		const int place = core_find_index_in_all_cars(id);
		all_cars[place].~car();
		car_num--;
		for (int i = place; i < car_num; i++) {
			all_cars[i] = all_cars[i + 1];
		}
		all_cars[car_num].violations = nullptr;
		all_cars[car_num].id = -1;
		all_cars[car_num].price = -1;
		cout << "\nSuccess!\n" << endl;
	}

	void controls::remove_violation(int code) {
		vector<found_violation> found = core_find_violation(code);
		int size = found.size();
		for (int i = 0; i < size; i++) {
			found[i].entry->dealt = true;
		}
		cout << "\nSuccess!\n" << endl;
	}

	void controls::find_car() {
		string s;
		cin >> s;
		if (s == "id") {
			int id;
			cin >> id;
			find_car_id(id);
		}
		else {
			//Ŀǰû�б�Ŀ��ܡ�������˾��ɶ�Ĳ����Լ����ˡ�����֮ǰ�����ֵ����������ۡ�����
			int id;
			cin >> id;
			find_car_id(id);
		}
	}

	void controls::find_car_id(int id) {
		int place = core_find_index_in_all_cars(id);
		cout << "Detailed information on the car is as follows:\n";
		cout << "id:\t" << all_cars[place].id << '\n';
		cout << "type:\t" << all_cars[place].type << '\n';
		cout << "price:\t" << all_cars[place].price << '\n';
		cout << "driver:\t" << all_cars[place].driver << '\n' << endl;
	}

	void controls::find_violation() {
		string s;
		cin >> s;
		if (s == "code") {
			int code;
			cin >> code;
			find_violation_code(code);
		}
		else if (s == "time") {
			int time;
			cin >> time;
			find_violation_time(time);
		}
		else if (s == "location") {
			string location;
			cin >> location;
			find_violation_location(location);
		}
		else throw runtime_error("find_violation: unknown command");
	}

	void controls::find_violation_code(int code) {
		vector<found_violation> found = core_find_violation(code);
		const int size = found.size();
		cout << "Cars with the follwing id are involved with the violation code " << code << ":\n";
		for (int i = 0; i < size; i++) {
			cout << all_cars[found[i].location_in_all_cars].id << '\t';
		}
		cout << endl;
		cout << "The following is the details of the violation:\n";
		cout << "time:\t\t" << found[0].entry->time << '\n';
		cout << "location:\t" << found[0].entry->location << '\n';
		if (found[0].entry->dealt) {
			cout << "The incident has been dealt with.\n" << endl;
		}
		else {
			cout << "The incident has not been not dealt with yet.\n" << endl;
		}
	}

	void controls::find_violation_time(int time) {
		vector<found_violation> found = core_find_violation_time(time);
		const int size = found.size();
		int* relations = new int[size + 1]();
		int* state = new int[size]();
		int cur_inc = 1;
		for (int i = 0; i < size; i++) {
			int code = found[i].entry->code;
			int rel = find_violation_find_rel(relations, code, cur_inc);
			if (rel == -1) {
				relations[cur_inc] = code;
				state[i] = cur_inc;
				cur_inc++;
			}
			else {
				state[i] = rel;
			}
		}
		cout << "The following violations occured at time " << time << ":\n" << endl;
		for (int j = 1; j < cur_inc; j++) {
			cout << "code:\t\t" << relations[j] << '\n';
			cout << "cars:\t\t";
			for (int k = 0; k < size; k++) {
				if (relations[k+1] == j) {
					cout << all_cars[k].id << '\t';
				}
			}
			cout << '\n';
			for (int k = 0; k < size; k++) {
				if (relations[k + 1] == j) {
					cout << "time:\t\t" << found[k].entry->time << '\n';
					cout << "location:\t" << found[k].entry->location << '\n';
					if (found[k].entry->dealt) {
						cout << "The incident has been dealt with.\n" << endl;
					}
					else {
						cout << "The incident has not been not dealt with yet.\n" << endl;
					}
					break;
				}
			}
		}
		delete[] relations;
		delete[] state;
	}

	void controls::find_violation_location(std::string location) {
		vector<found_violation> found = core_find_violation_loaction(location);
		const int size = found.size();
		int* relations = new int[size + 1]();
		int* state = new int[size]();
		int cur_inc = 1;
		for (int i = 0; i < size; i++) {
			int code = found[i].entry->code;
			int rel = find_violation_find_rel(relations, code, cur_inc);
			if (rel == -1) {
				relations[cur_inc] = code;
				state[i] = cur_inc;
				cur_inc++;
			}
			else {
				state[i] = rel;
			}
		}
		cout << "The following violations occured at location " << location << ":\n" << endl;
		for (int j = 1; j < cur_inc; j++) {
			cout << "code:\t\t" << relations[j] << '\n';
			cout << "cars:\t\t";
			for (int k = 0; k < size; k++) {
				if (relations[k + 1] == j) {
					cout << all_cars[k].id << '\t';
				}
			}
			cout << '\n';
			for (int k = 0; k < size; k++) {
				if (relations[k + 1] == j) {
					cout << "time:\t\t" << found[k].entry->time << '\n';
					cout << "location:\t" << found[k].entry->location << '\n';
					if (found[k].entry->dealt) {
						cout << "The incident has been dealt with.\n" << endl;
					}
					else {
						cout << "The incident has not been not dealt with yet.\n" << endl;
					}
					break;
				}
			}
		}
		delete[] relations;
		delete[] state;
	}

	int controls::find_violation_find_rel(int list[], int code, int range) {
		for (int i = 0; i < range; i++) {
			if (list[i] == code) {
				return i;
			}
		}
		return -1;
	}

	int controls::core_find_index_in_all_cars(int id) {
		for (int i = 0; i < car_num; i++) {
			if (all_cars[i].id == id) {
				return i;
			}
		}
		throw out_of_range("core_find_index_in_all_cars: cannot find specified id");
	}

	std::vector<controls::found_violation> controls::core_find_violation(int code) {
		violation* current;
		vector<found_violation> found;
		for (int i = 0; i < car_num; i++) {
			if (!all_cars[i].violations) {
				continue;
			}
			current = all_cars[i].violations;
			if (current->code == code) {
				found_violation temp{ current,i };
				found.push_back(temp);
			}
			while (current->next) {
				current = current->next;
				if (current->code == code) {
					found_violation temp{ current,i };
					found.push_back(temp);
				}
			}
		}
		if (found.size() == 0) {
			throw runtime_error("core_find_violation: code not found!");
		}
		return found;
	}

	std::vector<controls::found_violation> controls::core_find_violation_time(int time) {
		violation* current;
		vector<found_violation> found;
		for (int i = 0; i < car_num; i++) {
			if (!all_cars[i].violations) {
				continue;
			}
			current = all_cars[i].violations;
			if (current->time == time) {
				found_violation temp{ current,i };
				found.push_back(temp);
			}
			while (current->next) {
				current = current->next;
				if (current->time == time) {
					found_violation temp{ current,i };
					found.push_back(temp);
				}
			}
		}
		if (found.size() == 0) {
			throw runtime_error("core_find_violation_time: time not found!");
		}
		return found;
	}

	std::vector<controls::found_violation> controls::core_find_violation_loaction(std::string location) {
		violation* current;
		vector<found_violation> found;
		for (int i = 0; i < car_num; i++) {
			if (!all_cars[i].violations) {
				continue;
			}
			current = all_cars[i].violations;
			if (current->location == location) {
				found_violation temp{ current,i };
				found.push_back(temp);
			}
			while (current->next) {
				current = current->next;
				if (current->location == location) {
					found_violation temp{ current,i };
					found.push_back(temp);
				}
			}
		}
		if (found.size() == 0) {
			throw runtime_error("core_find_violation_location: location not found!");
		}
		return found;
	}

}