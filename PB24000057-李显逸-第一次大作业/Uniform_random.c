#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Uniform_random.h"

#define N 400
#define M 160000
//宏定义整个方形矩阵的规模 

#define delta 1e-4
//宏定义delta小量 

#define Lambda 500
//宏定义lambda的比例参数 

//二维数组储存所有距离值，作为均匀分布轴上的取值 
double dis[N][N] = {0.0};


int count;//计数变量 

//均匀分布所有取值的集合的生成函数 
double* create_value()
{
	//取值集合 
	double* val = calloc(M+1, sizeof(double)); 
	if(val == NULL)
	{
		return NULL; 
	} 
	int i,j;
	double lambda = (double)N / Lambda + delta;//带宽选择 【可调】 
	count = 1;//计数变量，从1开始，把0空出来给取值0.0 
	for(i = 0;i < N; i++)
	{
		for(j = 0;j < N;j++)
		{
		//调整时应当增大N和M，减小lambda，效果会更好，不过较为接近等差 
			if(j <= i + lambda && j >= i - lambda)//保留带状部分，从而筛掉中间多两边少的不均情况
			{
				//距离值求值时加上一个小量，防止有平方数和重复 
				dis[i][j] = sqrt(i*i + (j + delta)*(j + delta)) / sqrt(N*N + (N + delta)*(N + delta));
				//存入取值集合 ,从0到 N-1 
				val[count] = dis[i][j];
				//printf("%lf ",val[count]);
				count++;
			}
		}
	}
	double* value = calloc(count+1,sizeof(double));
	if(value == NULL)
	{
		return NULL;
	}
	for(i = 0;i <= count;i++)
	{
		value[i] = val[i];//用value数组转录一遍，去掉尾部的0 
	}
	free(val);
	return value; 
}

//生成服从均匀分布的随机变量
double create_Uniform(int range)
{
	double* Uniform_value = create_value();
	int r = rand() % (count + 1);
	double x = (range-1) * Uniform_value[r];
	return x;
}

