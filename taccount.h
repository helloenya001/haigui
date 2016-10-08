#pragma once
#include "stdafx.h"
#include "account.h"
#include <map>

//���ʻ�
//���е�ʵ�ʲ����ʻ������ڴ��ʻ�����
class taccount
{
private:
	static taccount *sigtAccount;
	int init(double tmoney);
	account* searchAccountL(char*gupID);  //���ڹ�ƱID���ҹ�Ʊ���ʻ�
public:
	taccount();
	~taccount();
	double totalmoney;
	double freemoney;//���ý��
	double usedmoney;//���˻��е���ʹ�ý��
	map<string, account*> subaccount; //��Ʊ�ʻ�
public:
	int addmoney(double money);
	int deletemoney(double money);
	int createAccount(char*gupID);  //����һ���չ�Ʊ�ʻ�
	int createAccount(char*gupID, double money);//����һ���������ʻ�
	int createAccount(char*gupID, int percent);//���ս����������ʻ���(���ý��+��ʹ�ý����б������㣬�����Ľ��<���ý��ʱ��ȡ���ý�
	int appendAccount(char*gupID, double moeny);//���ʻ���׷�ӽ�
	int deleteAccount(char*gupID);//ɾ��Ͷ���ʻ����黹���
	double getTotalmoney();//��ȡ��ǰ���ʻ��е����ý��+���ý��

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


