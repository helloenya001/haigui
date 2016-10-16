#include "stdafx.h"
#include "taccount.h"


taccount::taccount()
{
	totalmoney = 0;
	freemoney = 0;//可用金额
}


taccount::~taccount()
{
}

int taccount::init(double tmoney)
{
	totalmoney = tmoney;
	freemoney = tmoney;
	return 0;
}
account * taccount::searchAccountL(char * gupID)
{
	return nullptr;
}
int taccount::addmoney(double money)
{
	totalmoney += money;
	freemoney += money;
	return 0;
}

int taccount::deletemoney(double money)
{
	if (freemoney < money)
	{
		return -1;
	}
	else
	{
		totalmoney -= money;
		freemoney -= money;
	}
	return 0;
}

account* taccount::searchAccountL(char * gupID)
{
	string gupid = gupID;
	map<string, account*>::iterator it = subaccount.find(gupid);
	if (it != subaccount.end())
	{
		return it->second;
	}
	return NULL;
}

int taccount::createAccount(char * gupID)
{
	return createAccount(gupID, 0.0);
}

int taccount::createAccount(char * gupID, double money)
{
	account *pAcc = taccount::searchAccount(gupID);
	if (pAcc != NULL)
	{
		return 1;
	}
	//判断余额是否能够创建帐户
	if (this->freemoney < money)
	{
		return -2;
	}
	this->freemoney -= money;
	pAcc = new account();
	pAcc->init(money, gupID);
	string gupid = gupID;
	subaccount.insert(make_pair(gupid, pAcc));
	return 0;
}

//百分比创建帐户。
int taccount::createAccount(char * gupID, int percent)
{
	double money = this->totalmoney * percent;
	return createAccount(gupID, money);
}

int taccount::appendAccount(char * gupID, double money)
{
	account *pAcc = taccount::searchAccount(gupID);
	if (pAcc != NULL)
	{
		return -1;
	}
	pAcc->m_initmoney += money;
	pAcc->m_curmoney += money;
	return 0;
}

int taccount::deleteAccount(char * gupID)
{
	account *pAcc = taccount::searchAccount(gupID);
	if (pAcc == NULL)
	{
		return -1;
	}
	totalmoney += pAcc->m_curmoney-pAcc->m_initmoney;
	freemoney += pAcc->m_curmoney;
	subaccount.erase(gupID);
	delete(pAcc);
	return 0;
}
