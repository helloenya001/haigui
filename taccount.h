#pragma once
#include "stdafx.h"
#include "account.h"
#include <map>

//总帐户
//所有的实际操作帐户都基于此帐户创建
class taccount
{
private:
	static taccount *sigtAccount;
	int init(double tmoney);
	account* searchAccountL(char*gupID);  //基于股票ID查找股票仓帐户
public:
	taccount();
	~taccount();
	double totalmoney;
	double freemoney;//可用金额
	double usedmoney;//在账户中的已使用金额
	map<string, account*> subaccount; //股票帐户
public:
	int addmoney(double money);
	int deletemoney(double money);
	int createAccount(char*gupID);  //创建一个空股票帐户
	int createAccount(char*gupID, double money);//创建一个带金额的帐户
	int createAccount(char*gupID, int percent);//按照金额比例创建帐户。(可用金额+已使用金额）进行比例换算，换算后的金额<可用金额时，取可用金额。
	int appendAccount(char*gupID, double moeny);//向帐户中追加金额。
	int deleteAccount(char*gupID);//删除投资帐户，归还金额
	double getTotalmoney();//获取当前金额，帐户中的已用金额+可用金额

	static taccount* getTaccount(double tmoney) {
		if (sigtAccount = NULL)
		{
			sigtAccount = new taccount();
			sigtAccount->init(tmoney);
		}
		return sigtAccount;
	}
	static taccount*getTaccount(){
		return sigtAccount;
	}
	static account*searchAccount(char*gupID) {
		if (sigtAccount != NULL)
		{
			return sigtAccount->searchAccountL(gupID);
		}
		return NULL;
	}
};


