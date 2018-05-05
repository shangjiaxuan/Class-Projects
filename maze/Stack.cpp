#include "Stack.h"

using namespace std;

//template<typename type>
step Stack::pop_back() {
	if (empty) {
		throw std::runtime_error("Cyclic_Queue::pop_front: Cyclic_Queue is empty!");
	}
	step rtn = data[end];
	data[end] = step();
	if(end==0) {
		empty = true;
		return rtn;
	}
	end--;
	return rtn;
}

//template<typename type>
void Stack::push_back(step item) {
	if (empty) {
		end = 0;
		data[0] = item;
		empty = false;
		return;
	}
	end++;
	data[end] = item;
}

//template<typename type> 
bool Stack::exist(step& compared) {
	size_t i = 0;
	while(true) {
		if (data[i] == compared) {
			return true;
		}
		i++;
		if(i>=size()) {
			break;
		}
	}
	return false;
}

//template<typename type>
void Stack::print(std::ostream& ost) {
	size_t queue_size = size();
	for (size_t i = 0; i<queue_size; i++) {
		data[i].print(ost);
	}
	ost << endl;
}



//template<typename type>
//size_t Cyclic_Queue<type>::total_size;
