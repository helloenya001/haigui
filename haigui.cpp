// haigui.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#define DEFAULT_AVGM 20
#define DEFAULT_SL   2
#define DEFAULT_RP   10
#define DEFAULT_PP1  20
#define DEFAULT_PP2  50
#define DEFAULT_ADDI 0.5
#define DEFAULT_MU   4
#define DEFAULT_TMPDIR "./"
#define DEFAULT_LIMIT 0.01



//��ȡ��������
int initConig(T_CONFIG *pconfig)
{
	pconfig->avgm = DEFAULT_AVGM;
	pconfig->sl = DEFAULT_SL;
	pconfig->rp = DEFAULT_RP;
	pconfig->pp1 = DEFAULT_PP1;
	pconfig->pp2 = DEFAULT_PP2;
	pconfig->addi = DEFAULT_ADDI;
	pconfig->mu = DEFAULT_MU;
	pconfig->limit = DEFAULT_LIMIT;
	return 0;
}
/* �������������ļ�·��*/
int main(int argc, char ** argv)
{

	int ret = 0;
	char *infile = NULL;
	char *outfile = NULL;
	T_CONFIG config;
	/*if (argc <= 1)
	{
		printf("useage: haigui filename");
		return 0;
	}
	file = argv[1];
	*/
	infile = "E:\\work\\develop\\vc\\haigui\\log1113.csv";
	outfile = "E:\\work\\develop\\vc\\haigui\\log1113out.csv";
	//��ʼ������
	printf("init config\n");
	initConig(&config);

	//mode1 �������ݼ���
	BaseDataProc(config, infile, outfile);
	

    return 0;
}
