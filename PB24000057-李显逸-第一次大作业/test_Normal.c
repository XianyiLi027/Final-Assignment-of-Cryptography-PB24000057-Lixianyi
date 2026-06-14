#include <stdio.h>
#include <stdlib.h>
#include "test_Normal.h"
#include "Normal_random.h"
#include <math.h>

#define M 10000
//测试数据集规模

#define k 100
//卡方拟合优度检验划分区间数 

double data_N[M] = {0.0};
//根据宏定义调整测试数据集的规模大小 



void test_Normal_moment(int range)//测评方式：一阶矩和二阶矩检验 
{
	int i;
	for(i=0;i < M;i++)//生成数据集 
	{
		data_N[i] = create_Normal(range);
	} 
	
	double alpha = 0.05;//置信度【可调】 
	double U_standard_alpha = 1.95;//标准正态分布临界值 【可调】 
	double Chi_Square_standard_9999_high = 10298.4; //临界卡方（9999）上端值【可调】 
	double Chi_Square_standard_9999_low = 9706.6; //临界卡方（9999）下端值【可调】 
	double Chi_Square_standard_9999 = 10164.5;//卡方（9999）的右侧拒绝域的左端点【可调】 
	
	double E = 0.0;//样本均值
	double Var = 0.0;//样本方差
	double E_standard = (range - 1) / 2.0;//标准均值 range=100时 49.5 
	double Var_standard = (range-1)*(range-1) / 36.0;//标准方差 range=100时 272.25
	 
	for(i=0;i < M;i++)
	{
		E += (double)data_N[i] / M; 
	}
	for(i=0;i < M;i++)
	{
		Var += (data_N[i] - E)*(data_N[i] - E) / (M - 1); 
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





