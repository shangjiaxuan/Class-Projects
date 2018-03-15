//一点想法：用两个bool的数组存储每个路口可以出入的状态，用它算出要开多大的二维数组储存信息

#include "main.h"
#include "Test.h"
//#include "Test.h"

using namespace std;

using namespace Crossing;

int main(){
	Roads current;
	relations rel{ current };// = relations{ possi,possi };
	rel.find_relations();
	rel.print_rel();
	system("pause");
    return 0;
}

