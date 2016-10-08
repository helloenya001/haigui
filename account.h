#pragma once
#include <vector>

#define  ACCOUNT_BUY  0
#define  ACCOUNT_SELL 1
typedef struct t_transaction
{
	char date[32];// ����ʱ��
	int type;  //������������������٣����������
    double cache;//���ѽ��
	int gupnum; //����Ĺ�Ʊ�ݶ�
	double guvalue;//��Ʊ�۸�
}T_TRANS;

typedef struct t_tval
{
	char date[32]  ;// ����ʱ��
	double cache;   //���
	int gupnum;    //��Ʊ�ݶ�
	double guvalue;//��Ʊ�۸�
}T_TVAL;

class account
{
public:
	double m_initmoney;//��ʼ���
	char m_gupid[32];//��Ʊid
	int  m_curgupnum;    //ʣ���Ʊ�ݶ�
	double m_curmoney;//��ǰ���
	vector<T_TRANS> translist;
	vector<T_TVAL>m_tval;//ÿ��Ĺ�Ʊ��ֵ
protected:
	int operater(int type, char *pdate, double cache, int gupnum, double guvalue);//����
public:
	account();
	~account();
	int init(double money,char* gupid);
	int buygup(char *pdate, double cache, int gupnum, double guvalue);//����
	int sellgup(char *pdate, double cache, int gupnum, double guvalue);//����
};

