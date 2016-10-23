#include "stdafx.h"
#include "trader.h"
#include"taccount.h"
#include "stringproc.h"
#include "basedataproc.h"
#include "decision.h"

trader::trader()
{
	m_init = 0;
	m_taccount = taccount::getTaccount(0);
	m_decision = new decision();
}

trader::~trader()
{
}
int trader::init(T_CONFIG config, char* infile, char* logfile,char*desionlog,char*gupid)
{
	if (m_init == 0)
	{
		m_config = config;
		int ret = m_fp.InitFile(infile, FILE_READ);
		if (ret != 0)
		{
			return -1;
		}
		else
		{
			m_init = 1;
		}
		ret = m_logfp.InitFile(logfile, FILE_WRITE);
		if (ret != 0)
		{
			return -2;
		}
		else
		{
			m_init = 1;
		}
		strcpy_s(m_gupid, gupid);
		m_decision->init(config,gupid, config.mu,this, desionlog);
	}
	return 0;
}

int trader::addmoney(double money)
{
	m_taccount->addmoney(money);
	return 0;
}
int trader::formatdata(char*fl, T_OUTPUT_DATA *pOuteData)
{
	/*逗号分隔切词*/
	int ret = 0;
	vector<string>subs;
	ret = strssplit(fl, strlen(fl), ',', subs);
	if (ret <= 0)
	{
		return -1;
	}
	if (subs.size() <9)
	{
		return -2;
	}
	sprintf_s(pOuteData->t_org.timestamp, "%s", subs[0].c_str());
	pOuteData->t_org.opened = atof(subs[1].c_str());
	pOuteData->t_org.max = atof(subs[2].c_str());
	pOuteData->t_org.min = atof(subs[3].c_str());
	pOuteData->t_org.closing = atof(subs[4].c_str());
	pOuteData->p1 = atof(subs[5].c_str());;
	pOuteData->p2 = atof(subs[6].c_str());;
	pOuteData->minten = atof(subs[7].c_str());;
	pOuteData->n = atof(subs[8].c_str());;
	return 0;
}

int trader::proc()
{	
	//读取股市文件
	char *fl = NULL;
	int ret = 0;
	while (fl = m_fp.readNextline())
	{
		T_OUTPUT_DATA outData;

		ret = formatdata(fl, &outData);
		if (ret != 0)
		{
			continue;
		}
		m_decision->judge(outData);
		/*for (int i = 0;i < subs.size();i++)
		{
		printf("%s\n", subs[i].c_str());
		}*/
	}
	//数据输入，调用决策系统进行决策
	return 0;
}

//处理,将帐户中的钱转换为股票
//严重问题。由于上涨导致金额不足。
int trader::deal_notice(int opernum, char*pdate, int oper, char * gupid, int gupnum, double guvalue)
{
	account *pAcc = taccount::searchAccount(m_gupid);
	if (pAcc == NULL)
	{
		return -1;
	}
	if (oper == ACCOUNT_BUY)
	{
		pAcc->buygup(pdate, gupnum, guvalue);
	}
	else
	{
		pAcc->sellgup(pdate, gupnum, guvalue);
	}
	traderlog(pdate,oper, gupid,guvalue);
	m_decision->m_dealingtimes++;
	return 0;
}

//按照比例创建帐户
int trader::create_account(int opernum, char*pdate, char* gupid, double percent)
{
	int ret = m_taccount->createAccountBypercent(gupid, percent);
	if (ret != 0)
	{
		return -1;
	}
	return 0;
}

int trader::close_account(int opernum, char*pdate, char* gupid, int closestate, double guvalue)
{
	//卖出全部股票，修改
	account *pAcc = taccount::searchAccount(m_gupid);
	if (pAcc == NULL)
	{
		return -1;
	}
	pAcc->sellall(pdate,guvalue);
	m_decision->setstate(closestate);
	traderlog(pdate, closestate,gupid,guvalue);
	//销毁交易帐户，返回资金
	m_taccount->deleteAccount(gupid);
	return 0;
}

//日志:时间戳，操作，总价值金额，总可用金额，帐户名称，帐户初始金额，帐户金额，帐户现金,股票金额，股票数,股价
int trader::traderlog(char * pdate, int oper, char * gupid,double guvalue)
{
	static int init = 0;
	char line[1000] = { 0 };
	char*opers = "未知";
	if (init == 0)
	{
		snprintf(line, 1000, "时间戳,操作,总价值金额,总可用金额,帐户名称,帐户初始金额,帐户当前市值,帐户现金,股票市值,股票数,股价");
		m_logfp.saveline(line);
		init = 1;
	}
	account *pAcc = taccount::searchAccount(gupid);
	if (pAcc == NULL)
	{
		return -1;
	}
	switch (oper)
	{
	case ACCOUNT_BUY:
		opers = "购买";
		break;
	case ACCOUNT_SELL:
		opers = "卖出";
		break;
	case  ACCOUNT_CLOSE_TUPO:
		opers = "突破退出";
		break;
	case ACCOUNT_CLOSE_ZHISUN:
		opers = "止损退出";
		break;
	}
	snprintf(line, 1000, "%s,%s,%f,%f,%s,%f,%f,%f,%f,%d,%f", pdate, opers,m_taccount->getAccountTotalValue(),m_taccount->freemoney,gupid,pAcc->m_initmoney,pAcc->getAccountvalue(),pAcc->m_curmoney,pAcc->m_curgupnum*guvalue, pAcc->m_curgupnum, guvalue);
	m_logfp.saveline(line);
	return 0;
}
