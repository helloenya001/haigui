#include "stdafx.h"
#include "decision.h"


decision::decision()
{
    int test2 = 0;
	m_accountstate = STATE_NOACCOUNT;
	m_opernum = 0;
}


decision::~decision()
{
}

int decision::init(T_CONFIG config, char* gupid, int maxdealnum, deal_notice_func_t dealfunc, create_account_func_t creatfunc, close_account_func_t closefunc)
{
	strcpy(m_gupid, gupid);
	m_maxdealing = maxdealnum;
	m_deal_func = dealfunc;
	m_create_func = creatfunc;
	m_close_func = closefunc;
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
	
	//����ͷ�絥λ
	//����1%�����ʻ�
    int ret = m_create_func(0,m_config.limit*m_config.mu);
	if (ret == 0)
	{
		//1.ȡ���ʽ��ȡ� 2.����һ��ͷ��ɹ�����Ʊ����
		account *pAcc = taccount::searchAccount(m_gupid);
		if (pAcc == NULL)
		{
			return -1;
		}
		m_accountstate = STATE_DEALING;
		m_opernum++;
		pAcc->
		m_deal_func(m_opernum, ACCOUNT_BUY, m_gupid, gunum, guvalue);
	}
	
	return 0;
}

int decision::judgeDealing(account*pAcc, T_OUTPUT_DATA outData)
{
	return 0;
}


int decision::judge(T_OUTPUT_DATA outData)
{
	/*  1. ȷ�ϲ�ˢ�µ�ǰ״̬��
	    2. ����״̬�뵱ǰ������Ϣ�ж���һ������
	*/
	account *pAcc = taccount::searchAccount(m_gupid);
	//�����ж�
	if (pAcc == NULL)
	{
		return judgeEnter(outData);
	}
	else //�����ж�
	{
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
}
