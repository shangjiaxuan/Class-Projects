#include "Cyclic_Queue.h"

using namespace std;

template<typename type>
type Cyclic_Queue<type>::pop_front() {
	if(empty) {
		throw std::runtime_error("Cyclic_Queue::pop_front: Cyclic_Queue is empty!");
	}
	type rtn = data[start];
	data[start] = type();
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

template<typename type>
void Cyclic_Queue<type>::push_back(type item) {
	if(empty) {
		start = 0;
		end = 0;
		data[0] = item;
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

template<typename type>
size_t Cyclic_Queue<type>::total_size;

template class Cyclic_Queue<step>;

