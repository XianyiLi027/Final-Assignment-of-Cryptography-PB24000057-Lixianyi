#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Normal_random.h"
#include "Uniform_random.h"



//根据中心极限定理，独立的均匀分布随机变量的和是正态分布 

//生成服从标准正态分布的随机变量
double create_standardNormal()
{
	int i;
	double X = 0.0;//服从标准正态分布的随机变量 
	for(i = 0;i < 12;i++)
	{
		X += create_Uniform(2);//12个独立同分布的均匀分布的和 
	}
	X -= 6;//均值归到0
	return X; 
} 



//生成服从[0,N-1]上正态分布的随机变量 
double create_Normal(int N)
{
	double x = (double)(create_standardNormal() * (N - 1)) / 6 + (N - 1) / 2;//均值（N-1）/2，方差（N-1）/6 
	if(x < 0 || x > N - 1)
	{
		return create_Normal(N);//对于超出范围的数据重采样 
	}
	else
	{
		return x;	
	}
}



