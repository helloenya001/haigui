#include "stdafx.h"
#include "decision.h"
#include "trader.h"
#include "taccount.h"

decision::decision()
{
	m_accountstate = STATE_NOACCOUNT;
	m_opernum = 0;
}


decision::~decision()
{
}

int decision::init(T_CONFIG config, char* gupid, int maxdealnum, trader *ptrader)
{
	strcpy_s(m_gupid, gupid);
	m_ptrade = ptrader;
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
	//按照1%金额*头寸额度创建帐户  
    int ret = m_ptrade->create_account(0, outData.t_org.timestamp,m_gupid,m_config.limit*m_config.mu);
	if (ret == 0)
	{
		//1.取得资金额度。 2.计算一个头寸可购买点股票数量
		account *pAcc = taccount::searchAccount(m_gupid);
		if (pAcc == NULL)
		{
			return -1;
		}
		pAcc->setcurgavlue(outData.t_org.closing);
                m_oneToucun = pAcc->m_initmoney/m_config.mu; //一个头寸对应投资金额
                guvalue = outData.t_org.closing;
                gunum = (int)(m_oneToucun/guvalue);
                m_nextdealingLevel = guvalue+ outData.n/2; //下次投入门限值
		m_accountstate = STATE_DEALING;
		m_opernum++;
		
		m_ptrade->deal_notice(m_opernum, outData.t_org.timestamp, ACCOUNT_BUY, m_gupid, gunum, guvalue);
	}
	
	return 0;
}

int decision::judgeDealing(account*pAcc, T_OUTPUT_DATA outData)
{
    int gunum;
    double guvalue;
    //判断是否要退出
    if( outData.t_org.closing<outData.minten)//突破
    {
		m_ptrade->close_account(m_opernum,outData.t_org.timestamp,pAcc->m_gupid,CLOSE_TUPO,outData.t_org.closing);
        return 0;
    }
    else if( (outData.t_org.max - outData.t_org.min)/outData.n >m_config.sl ) //止损
    {
		m_ptrade->close_account(m_opernum, outData.t_org.timestamp, m_gupid,CLOSE_ZHISUN,outData.t_org.closing);
        return 0;
    }
    
    //达到最大交易次数,终止
    if(pAcc->getdealnum()>=m_config.mu)
    {
        return 0;
    }
    //是否要交易,涨幅超过下次投入门限值
    if( outData.t_org.closing > this->m_nextdealingLevel)
    {
        m_oneToucun = pAcc->m_initmoney/m_config.mu; //一个头寸对应投资金额
        guvalue = outData.t_org.closing;
        gunum = (int)(m_oneToucun/guvalue);
        m_nextdealingLevel = guvalue+ outData.n/2; //下次投入门限值
        m_opernum++;
		m_ptrade->deal_notice(m_opernum, outData.t_org.timestamp, ACCOUNT_BUY, m_gupid, gunum, guvalue);
    }
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
		pAcc->setcurgavlue(outData.t_org.closing);
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
	return 0;
}
