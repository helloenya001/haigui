#include "stdafx.h"
#include "basedataproc.h"
#include "stringproc.h"


#define MAX(a,b) (a)>(b)?(a):(b)
static basedataproc cbaseproc;

basedataproc::basedataproc()
{
	m_file = NULL;
	m_init = 0;
	memset((void*)&m_p, 0, sizeof(m_p)*P_NUM);
	memset((void*)&m_mintem, 0, sizeof(m_mintem));
	memset((void*)&m_tn, 0, sizeof(m_tn));
	for (int i = 0;i < P_NUM;i++)
	{
		m_p[i].cur = -1; //当前值初始化为0.
		m_p[i].maxIndex = -1;
	}
	m_mintem.cur = -1;
	m_mintem.maxIndex = -1;
}

basedataproc::~basedataproc()
{
}

int basedataproc::init(T_CONFIG config, char* infile,char* outfile)
{
	if (m_init == 0)
	{
		m_config = config;
		m_file = infile;
		m_outfile = outfile;
		int ret = m_fp.InitFile(m_file, FILE_READ);
		if (ret != 0)
		{
			return -1;
		}
		else
		{
			m_init = 1;
		}
		ret = m_outfp.InitFile(outfile, FILE_WRITE);
		if (ret != 0)
		{
			return -2;
		}
		else
		{
			m_init = 1;
		}
		m_p[0].mask = config.pp1+1; //当前值不计算，保留值为 n+1日数据
		m_p[1].mask = config.pp2+1;
		m_mintem.mask = config.rp + 1;
	}
	
	return 0;
}

// 20日N值为前20日每日TR的求和平均，第21日开始按照移动平均进行计算
int basedataproc::ATR(T_ORGIN_DATA Orgind)
{
	if (m_tn.dateCount == 0)
	{
		m_tn.lastclosing = Orgind.min;
	}
	double HL = (Orgind.max - Orgind.min);
	double HP = abs(Orgind.max - m_tn.lastclosing);
	double PL = abs(m_tn.lastclosing - Orgind.min);
	double TR = MAX(HL, MAX(HP, PL));
	if (m_tn.dateCount < 21)
	{
		m_tn.N += TR / 20;
	}
	else
	{
		m_tn.N = (19 * m_tn.N + TR) / 20;
	}
	m_tn.dateCount++;
	m_tn.lastclosing = Orgind.closing;
	return 0;
}
int basedataproc::Min10(double cur)
{
	double minv = 9999999;
	long minIndex = -1;

	int oldcur = m_mintem.cur;
	if (m_mintem.mask == 0)
	{
		return -1;
	}
	m_mintem.readnum++;
	m_mintem.cur++;
	//如果存储数据量达到mask量, 起始索引++
	if (m_mintem.mask <= m_mintem.cur)
	{
		m_mintem.start++;
	}

	m_mintem.pinfo[m_mintem.cur % m_mintem.mask] = cur;
	if (m_mintem.cur < m_mintem.mask - 1)  //数据不足，无需计算
	{
		return -2;
	}
	//最大值索引无效时，重新查找最小值，当前值不参与计算
	if ((m_mintem.maxIndex < m_mintem.start) || m_mintem.maxIndex == -1)
	{
		for (long j = m_mintem.start;j < m_mintem.cur;j++)
		{
			long index = j % m_mintem.mask;
			if (minv > m_mintem.pinfo[index])
			{
				minv = m_mintem.pinfo[index];
				minIndex = j;
			}
		}
		m_mintem.maxIndex = minIndex;
		m_mintem.maxv = minv;

	}
	else
	{
		if (m_mintem.maxv > m_mintem.pinfo[oldcur % m_mintem.mask])
		{
			m_mintem.maxv = m_mintem.pinfo[oldcur % m_mintem.mask];
			m_mintem.maxIndex = oldcur;
		}
	}
	return 0;
}
//当前值不用于计算。计算前n日的最高值
int basedataproc::updateP( double cur)
{
	double maxv = -999999.0;
	long maxIndex = -1;
	for ( long i = 0;i < 2;i++)
	{
		maxv = -999999.0;
		maxIndex = -1;

		int oldcur = m_p[i].cur;
		if (m_p[i].mask == 0)
		{
			continue;
		}
		m_p[i].readnum++;
		m_p[i].cur++;
		//如果存储数据量达到mask量, 起始索引++
		if(m_p[i].mask <= m_p[i].cur)
		{
			m_p[i].start++;
		}
		
		m_p[i].pinfo[m_p[i].cur % m_p[i].mask] = cur;
		if (m_p[i].cur < m_p[i].mask-1)  //数据不足，无需计算
		{
			continue;
		}
		//最大值索引无效时，重新查找最大值，当前值不参与计算
		if ((m_p[i].maxIndex < m_p[i].start)|| m_p[i].maxIndex == -1)
		{
			for ( long j = m_p[i].start;j < m_p[i].cur;j++)
			{
				long index = j % m_p[i].mask;
				if (maxv < m_p[i].pinfo[index])
				{
					maxv =m_p[i].pinfo[index];
					maxIndex = j;
				}
			}
			m_p[i].maxIndex = maxIndex;
			m_p[i].maxv = maxv;

		}
		else 
		{
			if (m_p[i].maxv < m_p[i].pinfo[oldcur % m_p[i].mask])
			{
				m_p[i].maxv = m_p[i].pinfo[oldcur % m_p[i].mask];
				m_p[i].maxIndex = oldcur;
			}
		}
	}
	return 0;
}
int basedataproc::formatdata(vector<string>subs, T_ORGIN_DATA *pOrgind)
{
	if( subs.size() <5)
	{
		return -1;
	}
	sprintf_s(pOrgind->timestamp, "%s", subs[0].c_str());
	pOrgind->opened = atof(subs[1].c_str());
	pOrgind->max = atof(subs[2].c_str());
	pOrgind->min = atof(subs[3].c_str());
	pOrgind->closing = atof(subs[4].c_str());
	return 0;
}
//输出数据
int basedataproc::outputline(T_ORGIN_DATA orgin_d)
{
	char line[400] = { 0 };
	T_OUTPUT_DATA to;
	to.t_org = orgin_d;
	to.minten = getMinten();
	to.n = getMn();
	to.p1 = getp1();
	to.p2 = getp2();

	if (to.p1 < 0 || to.p2 < 0)
	{
		return -1;
	}
	
	//写文件
	snprintf(line, 399, "%s,%.2f,%.2f,%.2f%,%.2f,%.2f,%.2f,%.2f,%.2f\n", orgin_d.timestamp, orgin_d.opened, orgin_d.max, orgin_d.min, orgin_d.closing,to.p1,to.p2,to.minten,to.n);
	m_outfp.saveline(line);
}
//每行进行数据转换标记
int basedataproc::decodeline(vector<string>subs)
{
	T_ORGIN_DATA orgin_d;
	int ret = 0;

	ret = formatdata(subs, &orgin_d);
	if (ret != 0)
	{
		return -1;
	}
	updateP(orgin_d.closing);
	Min10(orgin_d.closing);
	ATR(orgin_d);
	outputline(orgin_d);
	return 0;
}

int basedataproc::decodefile()
{
	char *fl = NULL;
	int ret = 0;
	while (fl = m_fp.readNextline())
	{
		vector<string>subs;
		/*逗号分隔切词*/
		ret = strssplit(fl, strlen(fl), ',', subs);
		if (ret <= 0)
		{
			continue;
		}
		decodeline(subs);
		
		/*for (int i = 0;i < subs.size();i++)
		{
			printf("%s\n", subs[i].c_str());
		}*/
	}
	return 0;
}

double basedataproc::getp1()
{
	if (m_p[0].cur < m_p[0].mask)
	{
		return -1.0;
	}
	else
	{
		return m_p[0].maxv;
	}
}

double basedataproc::getp2()
{
	if (m_p[1].cur < m_p[1].mask)
	{
		return -1.0;
	}
	else
	{
		return m_p[1].maxv;
	}
}

double basedataproc::getMinten()
{
	if (m_mintem.cur <m_mintem.mask)
	{
		return -1.0;
	}
	else
	{
		return m_mintem.maxv;
	}

}
double basedataproc::getMn()
{
		return m_tn.N;
}
int BaseDataProc(T_CONFIG config, char* infile,char*outfile)
{
	cbaseproc.init(config, infile,outfile);

	//read file,逐行读取，切分，并写入文件
	return cbaseproc.decodefile();

	return 0;
}
