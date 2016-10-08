#pragma once

#include<fstream>
#include <string>

using namespace std;

#define FILE_LINE_BUF 1000
#define FILE_READ  1
#define FILE_WRITE 2
#define LINE_MIN  1

class fileproc
{
	ifstream m_fin;
	ofstream m_fout;
	int m_init;
	char* m_pfNext;
	char m_filepath[256];
	int  m_linenum;
	int m_procend;  //�Ƿ��Ѵ�����β�����Ϊ1 �����ѽ���
	char m_bufline[2000];
	int m_count; 
	int m_minsize;

public:
	fileproc();
	~fileproc();

	int InitFile(char*filepath,int opentype, int minsize = LINE_MIN);

	char* readNextline();

	int saveline(char* line);


};


