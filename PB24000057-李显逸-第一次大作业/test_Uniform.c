#include <stdio.h>
#include <stdlib.h>
#include "test_Uniform.h"
#include "Uniform_random.h"
#include <math.h>

#define M 10000
//测试数据集规模

#define k 100
//卡方拟合优度检验划分区间数 

double data[M] = {0.0};
//根据宏定义调整测试数据集的规模大小 


void test_Uniform_Chi_square(int range)//测评方式：卡方拟合优度检验 
{
	int i,j;
	for(i=0;i < M;i++)//生成数据集 
	{
		data[i] = create_Uniform(range);
	} 
	
	double district_len = (double)(range-1) / k;//划分区间长度
	int district_count[k] = {0};//每个区间的统计频数集合
	for(i=0;i < M;i++)
	{
		for(j=0;j < k;j++)
		{
			if(j * district_len <= data[i] && data[i] < (j+1) * district_len)//划分整个范围，分别统计k个区间内的频数 
			{
				district_count[j]++;
			}
		}
	} 
	double alpha = 0.05;//置信度【可调】 
	double Chi_Square_standard = 124.3; //临界卡方【可调】 
	double Chi_Square = 0.0;//卡方统计量 
	double E = (double)M/k;//期望频数 
	printf("k个区间的分布依次为：\n");
	for(i=0;i < k;i++)
	{
		Chi_Square += (district_count[i] - E)*(district_count[i] - E) / E;
		printf("%d ",district_count[i]);
	}
	printf("\n");
	
	printf("k值为%d，且当k值为%d时，取α=%lf ，临界卡方大小为%lf\n",k,k,alpha,Chi_Square_standard);
	printf("卡方统计量为：%lf\n",Chi_Square);
	if(Chi_Square < Chi_Square_standard)
	{
		printf("卡方统计量小于临界卡方大小，在%lf的置信度下认为随机数生成是比较均匀的。\n\n",alpha);
	}
	else
	{
		printf("卡方统计量大于等于临界卡方大小，在%lf的置信度下认为随机数生成是不够均匀的。\n\n",alpha);
	}
}


void test_Uniform_moment(int range)//测评方式：一阶矩和二阶矩检验 
{
	int i;
	for(i=0;i < M;i++)//生成数据集 
	{
		data[i] = create_Uniform(range);
	} 
	
	double alpha = 0.05;//置信度【可调】 
	double U_standard_alpha = 1.95;//标准正态分布临界值 【可调】 
	double Chi_Square_standard_9999_high = 10298.4; //临界卡方（9999）上端值【可调】 
	double Chi_Square_standard_9999_low = 9706.6; //临界卡方（9999）下端值【可调】 
	double Chi_Square_standard_9999 = 10164.5;//卡方（9999）的右侧拒绝域的左端点【可调】 
	
	double E = 0.0;//样本均值
	double Var = 0.0;//样本方差
	double E_standard = (range - 1) / 2.0;//标准均值 range=100时 49.5 
	double Var_standard = (range*range-1) / 12.0;//标准方差 range=100时 833.5
	 
	for(i=0;i < M;i++)
	{
		E += (double)data[i] / M; 
	}
	for(i=0;i < M;i++)
	{
		Var += (data[i] - E)*(data[i] - E) / (M - 1); 
	}
	
	printf("样本的均值和方差分别为：%lf, %lf\n",E,Var);
	double U_miu = fabs((double)(E - E_standard))/ sqrt(Var/M) ;//均值检验统计量——正态分布 
	double Chi_sigmasquare = (double)(M-1)*Var / Var_standard ;//方差检验统计量——卡方分布 
	printf("标准均值和方差分别为：%lf, %lf\n",E_standard,Var_standard);
	printf("检验统计量的值分别为：%lf, %lf\n",U_miu, Chi_sigmasquare);
	if(fabs(U_miu) < 1.95)
	{
		printf("样本均值的检验统计量落在置信度为%lf的标准正态分布置信区间(-%lf,%lf)中，认为样本比较均匀。\n",alpha,U_standard_alpha,U_standard_alpha);
	}
	else
	{
		printf("样本均值的检验统计量未落在置信度为%lf的标准正态分布置信区间(-%lf,%lf)中，认为样本不够均匀。\n",alpha,U_standard_alpha,U_standard_alpha);
	}
	if(Chi_sigmasquare  <= Chi_Square_standard_9999)
	{
		printf("样本方差的检验统计量小于置信度为%lf时临界卡方大小%lf，认为样本比较均匀。\n\n",alpha,Chi_Square_standard_9999);
	}
	else
	{
		printf("样本方差的检验统计量大于等于置信度为%lf时临界卡方大小%lf，认为样本不够均匀。\n\n",alpha,Chi_Square_standard_9999);
	}
}


//排序算法 
void sort(double *a, int n) 
{
    int i,j;
	for (i=0; i < n-1; i++)
    {
    	for (j=i+1; j < n; j++)
    	{
    		if (a[j] < a[i])
    		{
    			double t = a[i]; a[i] = a[j]; a[j] = t;
			}
		}
	}		
}
//求最大值的函数
double maximum(double *a, int n)  
{
    double m = a[0];
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
void test_Uniform_max_gap(int range)//测评方式：最大间隙检验 
{
	double gap[M + 1] = {0.0};//记录所有的间隔 
	
	int i,j;
	for(i=0;i < M;i++)//生成数据集 
	{
		data[i] = create_Uniform(range);
	}
	sort(data,M);
	gap[0] = data[0];
	for(i=1;i < M;i++)
	{
		gap[i] = data[i] - data[i-1];
	}
	gap[M] = range - 1 - data[M - 1];
	double max_gap = maximum(gap, M);
	printf("最大间隙为：%lf\n",max_gap);
	double exp = (3-sqrt(3))*(range - 1)/M;
	if(max_gap <= exp)
	{
		printf("最大间隙小于经验值（%lf），此时认为输出是均匀的。\n\n",exp);
	}
	else
	{
		printf("最大间隙大于经验值（%lf），此时认为输出是不够均匀的。\n\n",exp);		
	}
}


