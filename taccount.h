#pragma once
#include "stdafx.h"
#include "account.h"
#include <map>

//总帐户
//所有的实际操作帐户都基于此帐户创建
class taccount
{
public:

private:

	int init(double tmoney);
	account* searchAccountL(char*gupID);  //基于股票ID查找股票仓帐户
public:
	taccount();
	~taccount();
	double initmoney;//投入金额
	double totalmoney;//资金总量，投入到子帐户的金额在子帐户清空前是按照固定值计算的。
	double freemoney;//可用金额
	map<string, account*> subaccount; //股票帐户
public:
	int addmoney(double money);
	int deletemoney(double money);
	int createAccount(char*gupID);  //创建一个空股票帐户
	int createAccount(char*gupID, double money);//创建一个带金额的帐户
	int createAccount(char*gupID, int percent);//按照金额比例创建帐户。(可用金额+已使用金额）进行比例换算，换算后的金额<可用金额时，取可用金额。
	int appendAccount(char*gupID, double moeny);//向帐户中追加金额。
	int deleteAccount(char*gupID);//删除投资帐户，归还金额
	double getAccountTotalValue();//获取总资产价值。

	static taccount* getTaccount(double tmoney) {
		taccount *ptac = getTaccount();
		ptac->init(tmoney);
		return ptac;
	}
	static taccount*getTaccount(){
		static taccount sigtAccount;
		return &sigtAccount;
	}
	static account*searchAccount(char*gupID) {
		taccount *ptac = getTaccount();
		return ptac->searchAccountL(gupID);
	}
};


