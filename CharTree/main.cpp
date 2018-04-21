#include "CharTree.h"
#include "Booklist.h"
#include "UI.h"

using namespace std;

/*
int main_ori() {
	Book test;
	std::string s = "HELLO!";
	test.add(s);
	test.index.print_tokens(std::cout);

	test.save();
//	test.del(s);
	system("pause");
//	test.save();
	return 0;
}
*/


UI This;

int main(int argc, char* argv[]) {
	while(This.on) {
		try {
			This.UI_main();
		} catch(exception& e) {
			cerr << e.what() << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return 0;
}

int test_main() {
	This.add();
	return 0;
}
