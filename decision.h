#pragma once
#include "taccount.h"
#include "basedataproc.h"
#include "fileproc.h"

/*决策者和操盘手
决策者decision：基于现有股市行情和帐户投资信息，判断下一步的操作
操盘手：根据决策者的指示进行帐户投资操作。
*/
#define STATE_NOACCOUNT 0
#define STATE_DEALING   1
#define STATE_BREAKING  2
#define STATE_STOPLOSS  3



class trader;

class decision
{
public:
	decision();
	~decision();
	char m_gupid[32];   //股票id
	int  m_dealingtimes;//交易中投资次数
	int  m_accountstate;//交易状态
    double m_oneToucun; //一个头寸
    double m_nextdealingLevel;// 下一次投资的时机
	double m_stoploss;
	int m_opernum;
	T_CONFIG m_config;
	trader *m_ptrade;
	fileproc m_logfp;
protected:
	int resetstate();
	int judgeEnter(T_OUTPUT_DATA outData);// 入市判断
	int judgeDealing(account*pAcc, T_OUTPUT_DATA outData);//交易判断
	int log(T_OUTPUT_DATA to, int oper, int dealnum, double menxian, int gunum);
public:
	int init(T_CONFIG config,char* gupid,int maxdealnum, trader*ptrader, char* logfile);// 初始化一个决策者
	int judge(T_OUTPUT_DATA outData);// 策略决策 
	int getstate();
	int setstate(int state);


};

