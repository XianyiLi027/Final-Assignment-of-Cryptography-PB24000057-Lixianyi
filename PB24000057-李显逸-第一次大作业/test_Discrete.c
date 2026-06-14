#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "test_Discrete.h"

#define N 100
#define M 10000


//二维数组：第一列表示 各个数的频率 ，每行后面N列用来记录差值，最后取平均 
int a[N][M+2] = {0};

//一维数组：当前坐标，遍历的时候每个数刷新一次 
int moment[N] = {0};


void create_func_discrete()//离散随机数生成函数，并记录分布 
{
	int i,x;
	for(i=1;i <= M;i++)
	{
		x = rand() % N;
		a[x][0]++;//记录频数 
		if(moment[x] > 0)
		{
			a[x][i+1] = i - moment[x];
		}
		moment[x] = i;
	}
}

void test_Discrete_probability()//测评方式1：以频率观测各个数的出现概率 
{
	create_func_discrete();
	double b[N] = {0.0};
	int i; 
	for(i=0;i < N;i++)
	{
		b[i] = (double)a[i][0] / M;
		printf("第%d个数据(%d)出现的概率为：%lf\n",i+1,i,b[i]);
	}
	printf("\n\n");
}

void test_Discrete_repeatgap()//测评方式2：测试每个数重复出现间隔的分布 
{
	create_func_discrete(); 
	int i,j;
	double sum[N] = {0.0};//一维数组：计算每个数间隔的和 
	for(j=0;j < N;j++)
	{
		for(i=1;i <= M;i++)
		{
			if(a[j][i] > 0)
			{
				sum[j] += a[j][i];
			}
		}
		printf("第%d个数据的间隔均值：%lf\n",j + 1,(double)sum[j] / a[j][0]);
	}
}

void test_Discrete_Chi_square()//测评方式3：卡方拟合优度检验 
{
	create_func_discrete(); 
	int i;
	int k = 100;//区间数 
	double alpha = 0.05;//置信度【可调】 
	double Chi_Square_standard = 124.3; //临界卡方【可调】 
	double Chi_Square = 0.0;//卡方统计量 
	double E = (double)M/k;//期望频数 
	
	for(i=0;i < M/k;i++)
	{
		Chi_Square += (a[i][0] - E)*(a[i][0] - E) / E;//卡方统计量计算式 
	}
	printf("k值为%d，且当k值为%d时，取α=%lf ，临界卡方大小为%lf\n",k,k,alpha,Chi_Square_standard);
	printf("卡方统计量为：%lf\n",Chi_Square);
	if(Chi_Square < Chi_Square_standard)
	{
		printf("卡方统计量小于临界卡方大小，在%lf的置信度下认为随机数生成是比较均匀的。",alpha);
	}
	else
	{
		printf("卡方统计量大于等于临界卡方大小，在%lf的置信度下认为随机数生成是不够均匀的。",alpha);
	}
}

void test_Discrete_moment()//测评方式4：一阶矩和二阶矩检验 
{
	create_func_discrete(); 
	int i;
	double E = 0.0;//样本均值
	double Var = 0.0;//样本方差 
	double alpha = 0.05;//置信度【可调】 
	double U_standard_alpha = 1.95;//标准正态分布临界值 【可调】 
	double Chi_Square_standard_9999_high = 10298.4; //临界卡方（9999）上端值【可调】 
	double Chi_Square_standard_9999_low = 9706.6; //临界卡方（9999）下端值【可调】 
	double E_standard = (N - 1) / 2.0;//标准均值 N=100时 49.5 
	double Var_standard = (N*N-1) / 12.0;//标准方差 N=100时 833.5
	 
	for(i=0;i < N;i++)
	{
		E += (double)a[i][0]*i / M; 
	}
	for(i=0;i < N;i++)
	{
		Var += (double) a[i][0]*(i - E)*(i - E) / (M - 1); 
	}
	
	printf("样本的均值和方差分别为：%lf, %lf\n",E,Var);
	double U_miu = fabs((double)(E - E_standard))/ sqrt(Var/M) ;//均值检验统计量——正态分布 
	double Chi_sigmasquare = (double)(M-1)*Var / Var_standard ;//方差检验统计量——卡方分布 
	printf("标准均值和方差分别为：%lf, %lf\n",E_standard,Var_standard);
	printf("检验统计量的值分别为：%lf, %lf\n",U_miu, Chi_sigmasquare);
	if(fabs(U_miu) < 1.95)
	{
		printf("样本均值的检验统计量落在置信度为%lf的标准正态分布置信区间(-%lf,%lf)中，认为样本均匀。\n",alpha,U_standard_alpha,U_standard_alpha);
	}
	else
	{
		printf("样本均值的检验统计量未落在置信度为%lf的标准正态分布置信区间(-%lf,%lf)中，认为样本不够均匀。\n",alpha,U_standard_alpha,U_standard_alpha);
	}
	if(Chi_sigmasquare >= Chi_Square_standard_9999_low && Chi_sigmasquare <= Chi_Square_standard_9999_high)
	{
		printf("样本方差的检验统计量落在置信度为%lf的卡方分布置信区间(%lf,%lf)中，认为样本均匀。\n",alpha,Chi_Square_standard_9999_low,Chi_Square_standard_9999_high);
	}
	else
	{
		printf("样本方差的检验统计量未落在置信度为%lf的标准正态分布置信区间(%lf,%lf)中，认为样本不够均匀。\n",alpha,Chi_Square_standard_9999_low,Chi_Square_standard_9999_high);
	}
}



//求最大值的函数
int max(int *a, int n)  
{
    int m = a[0];
    int i;
    for (i=1;i < n; i++) 
	{
		if (a[i] > m)
		{
			m = a[i];
		}
	}
    return m;
}
void test_Discrete_max_gap()//测评方式5：最大间隙检验 
{
	printf("本测试方式取值：N = 10000，M = 2000\n");
	create_func_discrete(); 
	int record[M] = {0};//转录数据的数组 
	int gap[M + 1] = {0};//记录所有的间隔 
	
	int i,j;
	int count = 0;//计数变量 
	for(i=0;i < N;i++)
	{
		if(a[i][0] > 0)
		{
			for(j=count;j < count+a[i][0];j++)
			{
				record[j] = i;//该数组把数据转录下来方便排序和算最大值
			}
			count += a[i][0];
		}
	}
	gap[0] = record[0];
	for(i=1;i < M;i++)
	{
		gap[i] = record[i] - record[i-1];
	}
	gap[M] = N - 1 - record[M - 1];
	int max_gap = max(gap, M);
	printf("最大间隙为：%d\n",max_gap);
	double exp = (3-sqrt(3))*(N - 1)/M;
	if(max_gap <= exp)
	{
		printf("最大间隙小于经验值（%lf），此时认为输出是均匀的。",exp);
	}
	else
	{
		printf("最大间隙大于经验值（%lf），此时认为输出是不够均匀的。",exp);		
	}
}


