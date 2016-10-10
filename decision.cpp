#include "stdafx.h"
#include "decision.h"


decision::decision()
{
	m_accountstate = STATE_NOACCOUNT;
	m_opernum = 0;
}


decision::~decision()
{
}

int decision::init(T_CONFIG config, char* gupid, int maxdealnum, deal_notice_func_t dealfunc, create_account_func_t creatfunc, close_account_func_t closefunc)
{
	strcpy(m_gupid, gupid);
	m_maxdealing = maxdealnum;
	m_deal_func = dealfunc;
	m_create_func = creatfunc;
	m_close_func = closefunc;
	m_config = config;
	return 0;
}
int decision::judgeEnter(T_OUTPUT_DATA outData)
{
	int deal = 0;
	int gunum;
	double guvalue;
	if (outData.p2 < outData.t_org.closing)
	{
		deal = 1;
	}
	else if (outData.p1 < outData.t_org.closing)
	{
		deal = 1;
	}
	if (deal == 0)
	{
		return 0;
	}
	
	//计算头寸单位
	//按照1%创建帐户  
    int ret = m_create_func(0,m_config.limit*m_config.mu);
	if (ret == 0)
	{
		//1.取得资金额度。 2.计算一个头寸可购买点股票数量
		account *pAcc = taccount::searchAccount(m_gupid);
		if (pAcc == NULL)
		{
			return -1;
		}
		m_accountstate = STATE_DEALING;
		m_opernum++;
		pAcc->
		m_deal_func(m_opernum, ACCOUNT_BUY, m_gupid, gunum, guvalue);
	}
	
	return 0;
}

int decision::judgeDealing(account*pAcc, T_OUTPUT_DATA outData)
{
	return 0;
}


int decision::judge(T_OUTPUT_DATA outData)
{
	/*  1. 确认并刷新当前状态。
	    2. 基于状态与当前股市信息判断下一步操作
	*/
	account *pAcc = taccount::searchAccount(m_gupid);
	//入市判断
	if (pAcc == NULL)
	{
		return judgeEnter(outData);
	}
	else //交易判断
	{
		return judgeDealing(pAcc, outData);
	}
}

int decision::getstate()
{
	return m_accountstate;
}

int decision::setstate(int state)
{
	m_accountstate = state;
}
