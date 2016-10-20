#include "stdafx.h"
#include "account.h"


account::account()
{
	m_initmoney = 0.0;//初始金额
	m_gupid[0]=0;//股票id
	m_curgupnum = 0;    //剩余股票份额
	m_curmoney= 0.0;//当前金额
	m_curguvalue = 0.0;
}


account::~account()
{
}

int account::init(double money, char* gupid)
{
	m_initmoney = money;
	strcpy_s(m_gupid,gupid);
	m_curmoney = money;
	return 0;
}

int account::buygup(char *pdate, int gupnum, double guvalue)
{
	int type = ACCOUNT_BUY;
	return operater(type,pdate,gupnum,guvalue);

}
int account::sellgup(char *pdate,  int gupnum, double guvalue)
{
	int type = ACCOUNT_SELL;
	return operater(type, pdate,  gupnum, guvalue);
}
//卖出全部股票
int account::sellall(char * pdate, double guvalue)
{
	T_TRANS tval;
	double cache = m_curgupnum *guvalue;
	tval.type = ACCOUNT_SELL;
	tval.cache = cache;
	tval.gupnum = m_curgupnum;
	tval.guvalue = guvalue;

	m_curmoney += cache;
	m_curgupnum = 0;
	translist.push_back(tval);
	return 0;
}
int account::setcurgavlue(double guvalue)
{
	m_curguvalue = guvalue;
	return 0;
}
int account::operater(int type, char *pdate, int gupnum, double guvalue)
{
	T_TRANS tval;
	double cache = gupnum*guvalue;
	tval.type = type;
	tval.cache = cache;
	tval.gupnum = gupnum;
	tval.guvalue = guvalue;
	strcpy_s(tval.date, pdate);

	if (type == ACCOUNT_BUY)
	{
		cache = -cache;
	}
	else
	{
		gupnum = -gupnum;
	}
	if (m_curmoney + cache < 0.0)
	{
		return -1;
	}
	if (m_curgupnum + gupnum < 0)
	{
		return -2;
	}
	m_curmoney += cache;
	m_curgupnum += gupnum;
	translist.push_back(tval);
	return 0;
}
int account::getdealnum()
{
    return translist.size();
}

double account::getAccountvalue()
{
	return m_curmoney+ m_curgupnum*m_curguvalue;
}
