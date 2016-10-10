#pragma once
#include "fileproc.h"
#include <vector>

#define MAX_PITEM_NUM 200
#define P_NUM  2

#define ORGIN_DATA_NUM 5
//原始数据结构
// 2016-01-31,
typedef struct t_orgin_data
{
	char   timestamp[64];
	double opened;
	double max;
	double min;
	double closing;

}T_ORGIN_DATA;

typedef struct t_output_data
{
	T_ORGIN_DATA t_org;
	double p1;
	double p2;
	double minten;
	double n;
}T_OUTPUT_DATA;

typedef struct t_N
{
	long dateCount;
	double lastclosing;
	int N;
}T_N;

class basedataproc
{
	typedef struct st_pinfo
	{
		 long start; //起始索引
		 long cur;   //当前索引
		 long maxIndex; //最高值索引
		 long mask;  //掩码
		 long usenum;  //使用多少做循环
		 long readnum;  //已经读入多少数据
		double pinfo[MAX_PITEM_NUM];// 循环保存信息
		double maxv;  //最大值

	}T_PINFO;
	int m_init;
	char *m_file;
	char *m_outfile;
	T_CONFIG m_config;
	fileproc m_fp;
	fileproc m_outfp;

	T_PINFO m_p[P_NUM];
	T_PINFO m_mintem;
	T_N m_tn;
public:
	basedataproc();
	~basedataproc();
	int decodeline(vector<string>subs);
	int formatdata(vector<string>subs, T_ORGIN_DATA *pOrgin_d);
	int init(T_CONFIG config, char* infile,char*outfile); //初始化
	int decodefile();// 解析处理文件
	double getp1();
	double getp2();
	double getMinten();
	double getMn();
	int updateP(double cur);
	int Min10(double cur);
	int ATR(T_ORGIN_DATA Orgind);
	int outputline(T_ORGIN_DATA orgin_d);

};

