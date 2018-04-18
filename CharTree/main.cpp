#include "CharTree.h"
#include "Booklist.h"
#include "UI.h"

int main_ori() {
	Book test;
	std::string s = "HELLO!";
	test.add(s);
	test.index.print_tokens();

	test.save();
//	test.del(s);
	system("pause");
//	test.save();
	return 0;
}

int main(int argc, char* argv[]) {
	UI This;
	while(true) {
		This.UI_main();
	}

}

