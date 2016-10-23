// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>

#define  ACCOUNT_BUY  0
#define  ACCOUNT_SELL 1
#define  ACCOUNT_CLOSE_TUPO   2 //突破退出
#define  ACCOUNT_CLOSE_ZHISUN 3 //止损退出

using namespace std;


// TODO:  在此处引用程序需要的其他头文件
typedef struct _st_config
{
	int avgm; // 移动平均值,默认20日
	int sl;   // 止损幅度(stop loss) SL默认 2N
	int rp;   // 突破赎回点(ransom point) RP默认 10日最低
	int pp1;  // 申购点1（purchase point 1) PP1 默认 20日
	int pp2;  // 申购点2( purchase point 2）PP2 默认 50日
	double addi; // 追加投资间隔(additional investment) ADDI 默认 1/2N
	int mu;   // 最大投入头寸单位( max unit) MU 默认 4
	double limit; // 投入额度
	char tmpdir[255]; //存放临时文件的默认路径
}T_CONFIG;

extern int BaseDataProc(T_CONFIG config, char* infile,char*outfile);