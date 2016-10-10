#include "stdafx.h"
#include "fileproc.h"




fileproc::fileproc()
{
	m_init = 0;

}
fileproc::~fileproc()
{
	if (m_fin.is_open())
	{
		m_fin.close();
	}
	if (m_fout.is_open())
	{
		m_fout.close();
	}
}

int fileproc::InitFile(char*filepath, int opentype,int minsize)
{
	if (filepath == NULL)
	{
		return -1;
	}
	if (m_init != 0)
	{
		return -3;
	}
	m_minsize = minsize;
	strcpy_s(m_filepath, filepath);

	if (opentype == FILE_READ)
	{
		m_fin.open(m_filepath, ios::in | ios::binary);
		if (!m_fin.is_open())
		{
			printf("open erro\n");
			return -2;
		}
		m_init = FILE_READ;
	}
	else if (opentype == FILE_WRITE)
	{
		m_fout.open(m_filepath, ios::out | ios::binary);
		if (!m_fout.is_open())
		{
			printf("open erro\n");
			return -2;
		}
		m_init = FILE_WRITE;
	}
	else
	{
		return -3;
	}
	return 0;
}

char* fileproc::readNextline()
{
	if (m_init != FILE_READ)
	{
		return NULL;
	}
	while(!m_fin.eof())  //逐行读取，直到结束
	{
		m_fin.getline(m_bufline, sizeof(m_bufline)-1);
		if (strlen(m_bufline) < m_minsize)
		{
			continue;
		}
		m_count++;
		if (m_bufline[strlen(m_bufline) - 1] == '\r')
		{
			m_bufline[strlen(m_bufline) - 1] = 0;
		}
		return m_bufline;
	}
	return NULL;
	
}

int fileproc::saveline(char* line)
{
	if (m_init != FILE_WRITE)
	{
		return -1;
	}
	m_fout << line;
	return 0;
}