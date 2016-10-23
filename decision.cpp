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

int decision::init(T_CONFIG config, char* gupid, int maxdealnum, trader *ptrader, char* logfile)
{
	strcpy_s(m_gupid, gupid);
	m_ptrade = ptrader;
	m_config = config;
	int ret = m_logfp.InitFile(logfile, FILE_WRITE);
	if (ret != 0)
	{
		return -2;
	}
	return 0;
}
int decision::resetstate()
{
	m_dealingtimes = 0;
	m_accountstate = STATE_NOACCOUNT;//交易状态
	m_nextdealingLevel = 0.0;// 下一次投资的时机
	m_stoploss =0.0;
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
		resetstate();
		pAcc->setcurgavlue(outData.t_org.closing);
                m_oneToucun = pAcc->m_initmoney/m_config.mu; //一个头寸对应投资金额
                guvalue = outData.t_org.closing;
                gunum = (int)(m_oneToucun/guvalue);
                m_nextdealingLevel = guvalue+ outData.n/2; //下次投入门限值
				m_stoploss = guvalue - outData.n*m_config.sl;
		m_accountstate = STATE_DEALING;
		m_opernum++;
		
		m_ptrade->deal_notice(m_opernum, outData.t_org.timestamp, ACCOUNT_BUY, m_gupid, gunum, guvalue);
		log(outData, ACCOUNT_BUY, m_dealingtimes, m_nextdealingLevel, gunum);
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
		m_ptrade->close_account(m_opernum,outData.t_org.timestamp,pAcc->m_gupid, ACCOUNT_CLOSE_TUPO,outData.t_org.closing);
		log(outData, ACCOUNT_CLOSE_TUPO, 0,0,0);
        return 0;
    }
    else if( outData.t_org.closing < m_stoploss) //止损
    {
		m_ptrade->close_account(m_opernum, outData.t_org.timestamp, m_gupid, ACCOUNT_CLOSE_ZHISUN,outData.t_org.closing);
		log(outData, ACCOUNT_CLOSE_ZHISUN, 0, 0, 0);
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
		m_stoploss = guvalue - outData.n*m_config.sl; //止损退出值
        m_opernum++;
		m_ptrade->deal_notice(m_opernum, outData.t_org.timestamp, ACCOUNT_BUY, m_gupid, gunum, guvalue);
		log(outData, ACCOUNT_BUY, m_dealingtimes,m_nextdealingLevel, gunum);
    }
    return 0;
}

// 日期,基本信息,决策值,第几次交易,下次交易门限,当前股价,交易数量
int decision::log(T_OUTPUT_DATA to,int oper,int dealnum,double menxian, int gunum)
{
	static int init = 0;
	char line[2000] = { 0 };
	char*opers = "未知";
	char*title = "日期,开盘,最高,最低,收盘,20日最高,50日最高,10日最低,振幅N,决策值,第几次交易,下次交易门限,止损价,当前股价,交易数量";
	int ret = 0;
	T_ORGIN_DATA orgin_d = to.t_org;
	if (init == 0)
	{
		m_logfp.saveline(title);
		init = 1;
	}
	switch (oper)
	{
	case ACCOUNT_BUY:
		opers = (char*)"购买";
		break;
	case ACCOUNT_SELL:
		opers = (char*)"卖出";
		break;
	case  ACCOUNT_CLOSE_TUPO:
		opers = "突破退出.";
		break;
	case ACCOUNT_CLOSE_ZHISUN:
		opers = "止损退出.";
		break;
	}
	//基本信息
	ret+=snprintf(line+ret,2000-ret, "%s,%.2f,%.2f,%.2f%,%.2f,%.2f,%.2f,%.2f,%.2f,", orgin_d.timestamp, orgin_d.opened, orgin_d.max, orgin_d.min, orgin_d.closing, to.p1, to.p2, to.minten, to.n);
	ret+=snprintf(line+ret,2000-ret, "%s,%d,%f,%f,%f,%d",opers,dealnum,menxian,m_stoploss,to.t_org.closing,gunum);
	m_logfp.saveline(line);
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
