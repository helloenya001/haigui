#include "stdafx.h"
#include "trader.h"
#include"taccount.h"
#include "stringproc.h"
#include "basedataproc.h"

trader::trader()
{
	m_init = 0;
	m_taccount = taccount::getTaccount(0);
}

trader::~trader()
{
}
int trader::init(T_CONFIG config, char* infile, char* logfile,char*gupid)
{
	if (m_init == 0)
	{
		m_config = config;
		int ret = m_fp.InitFile(infile, FILE_READ);
		if (ret != 0)
		{
			return -1;
		}
		else
		{
			m_init = 1;
		}
		ret = m_logfp.InitFile(logfile, FILE_WRITE);
		if (ret != 0)
		{
			return -2;
		}
		else
		{
			m_init = 1;
		}
		strcpy(m_gupid, gupid);
		m_decision.init(config,gupid, config.mu,this->deal_notice,this->create_account,this->close_account);
	}
	return 0;
}

int trader::addmoney(double money)
{
	m_taccount->addmoney(money);
	return 0;
}
int trader::formatdata(char*fl, T_OUTPUT_DATA *pOuteData)
{
	/*逗号分隔切词*/
	int ret = 0;
	vector<string>subs;
	ret = strssplit(fl, strlen(fl), ',', subs);
	if (ret <= 0)
	{
		return -1;
	}
	if (subs.size() <9)
	{
		return -2;
	}
	sprintf_s(pOuteData->t_org.timestamp, "%s", subs[0].c_str());
	pOuteData->t_org.opened = atof(subs[1].c_str());
	pOuteData->t_org.max = atof(subs[2].c_str());
	pOuteData->t_org.min = atof(subs[3].c_str());
	pOuteData->t_org.closing = atof(subs[4].c_str());
	pOuteData->p1 = atof(subs[5].c_str());;
	pOuteData->p2 = atof(subs[6].c_str());;
	pOuteData->minten = atof(subs[7].c_str());;
	pOuteData->n = atof(subs[8].c_str());;
	return 0;
}

int trader::proc()
{	
	//读取股市文件
	char *fl = NULL;
	int ret = 0;
	while (fl = m_fp.readNextline())
	{
		T_OUTPUT_DATA outData;

		ret = formatdata(fl, &outData);
		if (ret != 0)
		{
			continue;
		}
		m_decision.judge(outData);
		/*for (int i = 0;i < subs.size();i++)
		{
		printf("%s\n", subs[i].c_str());
		}*/
	}
	//数据输入，调用决策系统进行决策
	return 0;
}

int trader::deal_notice(int opernum, int oper, char * gupid, int gupnum, double guvalue)
{
	return 0;
}

int trader::create_account(int opernum, double money)
{
	return 0;
}

int trader::close_account(int opernum)
{
	return 0;
}
