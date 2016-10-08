#pragma once
#include "taccount.h"
#include "fileproc.h"
#include "decision.h"
/*�����ߺͲ�����
  ������decision���������й���������ʻ�Ͷ����Ϣ���ж���һ���Ĳ���
  �����֣����ݾ����ߵ�ָʾ�����ʻ�Ͷ�ʲ�����
*/
class trader
{
public:
	trader();
	~trader();
	int m_init;
	T_CONFIG m_config;
	taccount *m_taccount;
	fileproc m_fp;
	fileproc m_logfp;
	char m_gupid[32];   //��Ʊid
	decision m_decision;
protected:
	int formatdata(char*fl, T_OUTPUT_DATA *pOuteData);
public:
	//
	int init(T_CONFIG config, char* infile, char* logfile,char*gupid);
	int addmoney(double money);
	int proc();
	int deal_notice(int opernum, int oper, char* gupid, int gupnum, double guvalue);
	int create_account(int opernum, double percent);
	int close_account(int opernum);
	
};

