#include "stdafx.h"
#include "stringproc.h"
#include<fstream>
#include <string>
#include <vector>


using namespace std;


class stringproc
{
public:
	stringproc()
	{

	}
	~stringproc()
	{
	}


	int strssplit(char* str,int len,char loc, vector<string>&subs)
	{
		char delims[] = ",";
		char *p = str;
		int itemlen = 0;
		for (int i = 0;i < len;i++)
		{
			if (str[i] == loc)
			{
				if (itemlen > 0)
				{
					string sitem;
					sitem.append(p, itemlen);
					subs.push_back(sitem);
				}
				itemlen = 0;
				if (i + 1 < len)
				{
					p = &str[i+1];
				}

			}
			else
			{
				itemlen++;
			}
		}
		if (itemlen > 0)
		{
			string sitem;
			sitem.append(p, itemlen);
			subs.push_back(sitem);
		}
		return subs.size();
	}

};
stringproc strpoc;
int strssplit(char* str, int len, char loc, vector<string>&subs)
{
	return strpoc.strssplit(str, len, loc, subs);
}
