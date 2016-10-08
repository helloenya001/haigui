#pragma once
#include <vector>

#define  ACCOUNT_BUY  0
#define  ACCOUNT_SELL 1
typedef struct t_transaction
{
	char date[32];// 操作时间
	int type;  //类型买还是卖，买金额减少，卖金额增加
    double cache;//花费金额
	int gupnum; //购买的股票份额
	double guvalue;//股票价格
}T_TRANS;

typedef struct t_tval
{
	char date[32]  ;// 操作时间
	double cache;   //金额
	int gupnum;    //股票份额
	double guvalue;//股票价格
}T_TVAL;

class account
{
public:
	double m_initmoney;//初始金额
	char m_gupid[32];//股票id
	int  m_curgupnum;    //剩余股票份额
	double m_curmoney;//当前金额
	vector<T_TRANS> translist;
	vector<T_TVAL>m_tval;//每天的股票价值
protected:
	int operater(int type, char *pdate, double cache, int gupnum, double guvalue);//操盘
public:
	account();
	~account();
	int init(double money,char* gupid);
	int buygup(char *pdate, double cache, int gupnum, double guvalue);//购买
	int sellgup(char *pdate, double cache, int gupnum, double guvalue);//卖出
};

