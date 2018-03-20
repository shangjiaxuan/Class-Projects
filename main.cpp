//一点想法：用两个bool的数组存储每个路口可以出入的状态，用它算出要开多大的二维数组储存信息

#include "main.h"
#include "Test.h"
#include "Colors.h"
//#include "Test.h"

using namespace std;

using namespace Crossing;

int main(){
	//让用户输入，并算出各种路线的关系
	Roads current;
	relations rel{ current };
	rel.find_relations();
	rel.print_rel();
	Colors color{rel};
	color.find_groups();
	color.print_result();
	system("pause");
    return 0;
}

