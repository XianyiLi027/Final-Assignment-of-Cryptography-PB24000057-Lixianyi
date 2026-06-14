#include <stdio.h>
#include <stdlib.h>
#include "test_Discrete.h"
#include "Uniform_random.h"
#include "Normal_random.h"

int main()
{
	test_Discrete_probability();//概率测试方法 
	test_Discrete_repeatgap();//重复间隔测试方法 
	test_Discrete_Chi_square();//卡方拟合优度测试方法 
	test_Discrete_moment();//一阶矩和二阶矩测试方法 
	test_Discrete_max_gap();//最大间隔测试方法 
	
	printf("对均匀分布生成函数的测试结果：\n");
	test_Uniform_Chi_square(100);
	test_Uniform_moment(100);
	test_Uniform_max_gap(100);
	printf("对正态分布生成函数的测试结果：\n");
	test_Normal_moment(100);

	return 0;
}
