#include "stdafx.h"
#include "taccount.h"


taccount::taccount()
{
	totalmoney = 0;
	freemoney = 0;//可用金额
	usedmoney = 0;
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

account* taccount::searchAccount(char * gupID)
{
	return NULL;
}

int taccount::createAccount(char * gupID)
{
	return 0;
}

int taccount::createAccount(char * gupID, double money)
{
	return 0;
}

int taccount::createAccount(char * gupID, int percent)
{
	return 0;
}

int taccount::appendAccount(char * gupID, double moeny)
{
	return 0;
}
