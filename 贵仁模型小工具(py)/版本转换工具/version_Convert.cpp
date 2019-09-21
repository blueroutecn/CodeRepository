#include"version_Convert.h"

void get_num(string file, int &crossnum, int &reachnum)
{
	FileInputStream fis(file);
	DataStreamReader reader(fis, file);

	while (!reader.isEnd())
	{
		reader.nextLine();
		if (reader.getDataCount() < 1) continue;
		if (reader.getString(0) == "[reach]")
		{
			reader.nextLine();
			crossnum += reader.getInteger(0);
			reachnum++;
		}
	}
}
void convert_HD(string infile, string outfile)
{
	FileInputStream fis(infile);
	DataStreamReader reader(fis, infile);

	int dmnum = 0, hdnum = 0;
	get_num(infile, dmnum, hdnum);
	cross *DM = new cross[dmnum];
	reach *HD = new reach[hdnum];
    reader.nextLine();
	int n = 0, i = 0;
	while (!reader.isEnd())
	{		
		if (reader.getDataCount() < 1) { reader.nextLine(); continue; }
		else if (reader.getString(0) == "[reach]")
		{
			reader.nextLine();
			int crossNum = reader.getInteger(0);
			reach &tempHD = HD[n];
			tempHD.hdID = n + 1;
			tempHD.crossNum = crossNum;
			tempHD.crossID = new string[crossNum];

			reader.nextLine();
			for (int j = 0; j < crossNum; j++, i++)
			{
				int nodeNum = reader.getInteger(0);
				cross &tempDM = DM[i];
				tempHD.crossID[j] = reader.getString(1);
				tempDM.name = reader.getString(1);
				tempDM.nodeNum = nodeNum;
				tempDM.node = new node[nodeNum];
				reader.nextLine();
				for (int k = 0; k < nodeNum; k++)
				{
					tempDM.node[k].x = reader.getDouble(0);
					tempDM.node[k].y = reader.getDouble(1);
					reader.nextLine();
				}
			}
			n++;
		}
		else reader.nextLine();
	}

	FileOutputStream out(outfile);
	out << "[Reach]" << endl;
	for (int i = 0; i < hdnum; i++)
	{
		int num = HD[i].crossNum;
		out << "HD" << HD[i].hdID << setw(10);
		for (int j = 0; j<num; j++)
		{
			out << HD[i].crossID[j] << setw(10);
		}
		out << "\r\n";
	}
	out << "\r\n";

	out << "[Cross]" << endl;
	for (int i = 0; i < dmnum; i++)
	{
		int num = DM->nodeNum;
		for (int j = 0; j < num; j++)
		{
			out << DM[i].name << setw(10) << DM[i].node[j].x << setw(10) << DM[i].node[j].y << "\r\n";
		}
	}

	delete[]DM;
	delete[]HD;
}
void convert_BC(string infile1, string infile2, string outfile2)
{
	FileInputStream fis1(infile1);
	DataStreamReader reader1(fis1, infile1);
	FileOutputStream out2(outfile2);

	int bcNum = 0;
	while (!reader1.isEnd())
	{
		reader1.nextLine();
		string key;
		if (reader1.getDataCount() < 1) continue;
		key = reader1.getString(0);
		if (key.at(0) != '!')
		{
			bcNum = reader1.getInteger(0);
			do
			{
				reader1.nextLine();
				if (reader1.getDataCount() < 1) continue;
				key = reader1.getString(0);
			} while (key.at(0) == '!'); break;
		}
	}

	out2 << "[BC]" << "\r\n";
	for (int i = 0; i < bcNum; i++)
	{
		out2 << "BC" << i + 1 << setw(10) << reader1.getInteger(2) << setw(10) << reader1.getString(1) << setw(10) << "XL" << i + 1 << "\r\n";
		reader1.nextLine();
	}out2 << "\r\n";

	FileInputStream fis2(infile2);
	DataStreamReader reader2(fis2, infile2);

	int len = 0;
	while (!reader2.isEnd())
	{
		reader2.nextLine();
		string key;
		if (reader2.getDataCount() < 1) continue;
		key = reader2.getString(0);
		if (key.at(0) != '!')
		{
			len = reader2.getInteger(0);
			do
			{
				reader2.nextLine();
				if (reader2.getDataCount() < 1) continue;
				key = reader2.getString(0);
			} while (key.at(0) == '!'); break;
		}
	}
	Time *XL = new Time[bcNum];
	for (int i = 0; i < bcNum; i++)
	{
		XL[i].xlname = i + 1;
		XL[i].time = new node[len];
	}
	for (int j = 0; j < len; j++)
	{
		for (int i = 0; i < bcNum; i++)
		{
			XL[i].time[j].x = reader2.getDouble(0);
			XL[i].time[j].y = reader2.getDouble(i + 1);
		}
		reader2.nextLine();
	}
	out2 << "[series]" << "\r\n";
	for (int i = 0; i < bcNum; i++)
	{
		out2 << "XL" << i + 1 << setw(10) << len << setw(10) << "hour" << setw(10) << 1 << "\r\n";//默认时间单位hour,每个变量按一行输入
		for (int j = 0; j < len; j++)
		{
			out2 << XL[i].time[j].y << setw(10);
		}out2 << "\r\n";
		for (int j = 0; j < len; j++)
		{
			out2 << XL[i].time[j].x << setw(10);
		}out2 << "\r\n";
		out2 << "\r\n";
	}
	delete[]XL;
}
