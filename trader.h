#pragma once
#include "taccount.h"
#include "fileproc.h"
#include "basedataproc.h"

class decision;
/*决策者和操盘手
  决策者decision：基于现有股市行情和帐户投资信息，判断下一步的操作
  操盘手：根据决策者的指示进行帐户投资操作。
*/
class trader
{
public:
	trader();
	~trader();
	int m_init;
	T_CONFIG m_config;
	taccount *m_taccount;
	fileproc m_fp;
	fileproc m_logfp;
	char m_gupid[32];   //股票id
	decision *m_decision;
protected:
	int formatdata(char*fl, T_OUTPUT_DATA *pOuteData);
public:
	//
	int init(T_CONFIG config, char* infile, char* logfile,char*gupid);
	int addmoney(double money);
	int proc();

	//opernum: 内部交易流水号
	//oper: 买卖
	//gupid:股票ID
	//gupnum:股票数量
	//guvalue:当前股票价格
	//pdata 触发时间
	int create_account(int opernum, char*pdate, char* gupid, double percent);
	int deal_notice(int opernum, char*pdate, int oper, char* gupid, int gupnum, double guvalue);
	int close_account(int opernum, char*pdate, char* gupid, int closestate, double guvalue);
	//记录交易日志
	int traderlog(char*pdate, int oper,char*gupid,double guvalue);
	
};

