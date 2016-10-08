#pragma once
#include "taccount.h"
#include "basedataproc.h"
/*�����ߺͲ�����
������decision���������й���������ʻ�Ͷ����Ϣ���ж���һ���Ĳ���
�����֣����ݾ����ߵ�ָʾ�����ʻ�Ͷ�ʲ�����
*/
#define STATE_NOACCOUNT 0
#define STATE_DEALING   1
#define STATE_BREAKING  2
#define STATE_STOPLOSS  3



//�����ص���
//opernum: �ڲ�������ˮ��
//oper: ����
//gupid:��ƱID
//gupnum:��Ʊ����
//guvalue:��ǰ��Ʊ�۸�
typedef int(*deal_notice_func_t)(int opernum,int oper, char* gupid,int gupnum, double guvalue); 
//�����ʻ�
typedef int(*create_account_func_t)(int opernum,double percent);
//�ر��ʻ�
typedef int(*close_account_func_t)(int opernum);

class decision
{
public:
	decision();
	~decision();
	char m_gupid[32];   //��Ʊid
	int  m_maxdealing;  //����״���
	int  m_dealingtimes;//������Ͷ�ʴ���
	int  m_accountstate;//����״̬
	deal_notice_func_t m_deal_func;
	create_account_func_t m_create_func;
	close_account_func_t m_close_func;
	int m_opernum;
	T_CONFIG m_config;
protected:

	int judgeEnter(T_OUTPUT_DATA outData);// �����ж�
	int judgeDealing(account*pAcc, T_OUTPUT_DATA outData);//�����ж�
public:
	int init(T_CONFIG config,char* gupid,int maxdealnum, deal_notice_func_t dealfunc, create_account_func_t creatfunc, close_account_func_t closefunc);// ��ʼ��һ��������
	int judge(T_OUTPUT_DATA outData);// ���Ծ��� 
	int getstate();
	int setstate(int state);

};

