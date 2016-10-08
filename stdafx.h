// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>

#define  ACCOUNT_BUY  0
#define  ACCOUNT_SELL 1

using namespace std;


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
typedef struct _st_config
{
	int avgm; // �ƶ�ƽ��ֵ,Ĭ��20��
	int sl;   // ֹ�����(stop loss) SLĬ�� 2N
	int rp;   // ͻ����ص�(ransom point) RPĬ�� 10�����
	int pp1;  // �깺��1��purchase point 1) PP1 Ĭ�� 20��
	int pp2;  // �깺��2( purchase point 2��PP2 Ĭ�� 50��
	double addi; // ׷��Ͷ�ʼ��(additional investment) ADDI Ĭ�� 1/2N
	int mu;   // ���Ͷ��ͷ�絥λ( max unit) MU Ĭ�� 4
	double limit; // Ͷ����
	char tmpdir[255]; //�����ʱ�ļ���Ĭ��·��
}T_CONFIG;

extern int BaseDataProc(T_CONFIG config, char* infile,char*outfile);