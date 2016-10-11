#pragma once
#include "taccount.h"
#include "basedataproc.h"
/*决策者和操盘手
决策者decision：基于现有股市行情和帐户投资信息，判断下一步的操作
操盘手：根据决策者的指示进行帐户投资操作。
*/
#define STATE_NOACCOUNT 0
#define STATE_DEALING   1
#define STATE_BREAKING  2
#define STATE_STOPLOSS  3

#define CLOSE_TUPO   0 //突破退出
#define CLOSE_ZHISUN 1 //止损退出

//操作回调，
//opernum: 内部交易流水号
//oper: 买卖
//gupid:股票ID
//gupnum:股票数量
//guvalue:当前股票价格
typedef int(*deal_notice_func_t)(int opernum,int oper, char* gupid,int gupnum, double guvalue); 
//创建帐户
typedef int(*create_account_func_t)(int opernum,double percent);
//关闭帐户
typedef int(*close_account_func_t)(int opernum,int closestate);

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
	deal_notice_func_t m_deal_func;
	create_account_func_t m_create_func;
	close_account_func_t m_close_func;
	int m_opernum;
	T_CONFIG m_config;
protected:

	int judgeEnter(T_OUTPUT_DATA outData);// 入市判断
	int judgeDealing(account*pAcc, T_OUTPUT_DATA outData);//交易判断
public:
	int init(T_CONFIG config,char* gupid,int maxdealnum, deal_notice_func_t dealfunc, create_account_func_t creatfunc, close_account_func_t closefunc);// 初始化一个决策者
	int judge(T_OUTPUT_DATA outData);// 策略决策 
	int getstate();
	int setstate(int state);

};

