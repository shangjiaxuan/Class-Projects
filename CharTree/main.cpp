#include "CharTree.h"
#include "Booklist.h"

int main() {
	Book test;
	std::string s = "HELLO!";
	test.add(s);
	test.print_tokens();
	test.save();
	test.del(s);
	system("pause");
	return 0;
}

