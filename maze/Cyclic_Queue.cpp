#include "Cyclic_Queue.h"

using namespace std;

//template<typename type>
step Cyclic_Queue::pop_front() {
	if(empty) {
		throw std::runtime_error("Cyclic_Queue::pop_front: Cyclic_Queue is empty!");
	}
	step rtn = data[start];
	data[start] = step();
	if (start == end) {
		empty = true;
		start = 0;
		end = 0;
		return rtn;
	}
	start++;
	if (start >= total_size) {
		start -= total_size;
	}
	return rtn;
}

//template<typename type>
void Cyclic_Queue::push_back(step item) {
	if(empty) {
		start = 0;
		end = 0;
		data[0] = item;
		empty = false;
		return;
	}
	end++;
	if(end>=total_size) {
		end -= total_size;
	}
	if(end==start) {
		throw runtime_error("Cyclic_Queue::push_back: queue overflow!");
	}
	data[end] = item;
}

//template<typename type> 
bool Cyclic_Queue::exist(step& compared) {
	size_t i = start;
	do {
		if(data[i]==compared) {
			return true;
		}
		i++;
		if(i>=total_size) {
			i -= total_size;
		}
	} while (i != end);
	return false;
}

//template<typename type>
void Cyclic_Queue::print(std::ostream& ost) {
	size_t queue_size = size();
	for(size_t i=0; i<queue_size; i++) {
		data[i].print(ost);
	}
	ost << endl;
}



//template<typename type>
//size_t Cyclic_Queue<type>::total_size;
