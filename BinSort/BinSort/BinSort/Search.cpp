#include "BinSort.h"

using namespace std;

void BinSort::UI() {
	int index;
	cout << "Please specify a index to search for:\n";
	cin >> index;
	search_verbose(index);
}

void BinSort::search_verbose(int index) {
	long long place = root;
	while (place >= 0) {
		if (data[place].data == index) {
			cout << "Index found!\n";
			cout << endl;
			return;
		}
		else if(data[place].data>index) {
			cout << "Index entry " << data[place].data << " larger than given index " << index << "! going to left child...\n";
			place = data[place].lchild;
		}
		else if (data[place].data<index) {
			cout << "Index entry " << data[place].data << " smaller than given index " << index << "! going to right child...\n";
			place = data[place].rchild;
		}
	}
	cout << "No Index found!\n";
	cout << endl;
}

void BinSort::print() {
	for (size_t i = 0; i < size; i++) {
		cout << "Data:\t" << data[i].data << '\n';
		cout << "Depth:\t" << data[i].depth << '\n';
		cout << "Parent:\t" << data[i].parent << '\n';
		cout << "lChild:\t" << data[i].lchild << '\n';
		cout << "rChild:\t" << data[i].rchild << '\n';
		cout << endl;
	}
}

