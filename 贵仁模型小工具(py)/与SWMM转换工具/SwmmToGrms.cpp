/*--- 头文件 ------------------------------------------------*/
#include<iomanip>
#include<algorithm>
#include<math.h>
#include<direct.h>
#include"texts.h"
#include"SwmmToGrms.h"



/*--- 命名空间 ------------------------------------------ ---*/
using std::ios;
using std::setw;
using std::endl;
using namespace SwmmToGrms;



/*--- 调试日志 -------------------------------------- -------*/
#ifdef __DEBUG
#include<windows.h>
using std::cout;
using std::endl;


int main()
{
	string text = "u are the best";
	Log* log = Log::GetLog();
	log->WriteLogFile(text);
	
	string swmm = "C:\\Users\\gr\\Desktop\\Examples\\RiverModel.inp";
	SwmmInp* inp = SwmmInp::GetSwmmInp(swmm);
	cout << "here1" << endl;


	//SwmmNode node(inp);
	//const vector<NODE>& _node = node.ReturnSwmmNode(); 
	//cout << "here2\n";
	//SwmmPipe pipe(inp);
	//const vector<PIPE>& _pipe = pipe.ReturnSwmmPipe(); 
	//cout << "here3\n";
	//SwmmTime time(inp); 
	//const vector<TIME>& _time = time.ReturnSwmmTime(); 
	//cout << "here4\n";
	SwmmTransect tran(inp);
	const vector<TRANSECT>& _tran = tran.ReaturnSwmmTransect();
	//SwmmOption option(inp);
	//const OPTION& _option = option.ReaturnSwmmOption();
	//cout << "here6\n";
	//const vector<NODESITE>& _site = SetNodeSite(_node, _pipe);
	//cout << "here7\n";


	//JunctionConvertor junc(_node, _site,_time);
	//cout << "here8\n";
	//HdConvertor hd(_tran, _pipe, _node, _site);
	//cout << "here9\n";
	//TimeConvertor bctime(_time);
	//cout << "here10\n";
	//SetConvertor set(_option, _node, _pipe);
	//cout << "here11\n";
	//SluiceConvertor sluice(_pipe, _site);
	//cout << "here12\n";
	//WeirConvertor weir(_pipe, _site);
	//cout << "here13\n";
	//PumpConvertor pump(_pipe, _site);
	//cout << "here14\n";


	//DstConvertor dst(junc.ReturnLocation(),hd.ReturnHd());
	//cout << "here15\n";
	//PondConvertor pond(_node, _site, junc.ReturnGrJunction());
	//cout << "here16\n";
	//cout << "-------------------------------\n";
	//

	//string grms = "F:\\temp";
	//GrmsWriter writer(grms);
	//writer.WriteHd(hd.ReturnHd());
	//cout << "hd writed\n" ;
	//writer.WriteDst(dst.ReturnDst());
	//cout << "dst writed\n";
	//writer.WriteJunction(junc.ReturnGrJunction());
	//cout << "junction writed\n";
	//writer.WriteLocation(junc.ReturnLocation());
	//cout << "location writed\n";
	//writer.WriteSeting(set.ReturnSet());
	//cout << "set writed\n";
	//writer.WriteManing(hd.ReturnManing());
	//cout << "manning writed.\n";
	//writer.writePsource(junc.ReturnPsource());
	//cout << "psource writed\n";
	//writer.WriteBcSection(junc.ReturnBcSection());
	//cout << "bccrosssection writed\n";
	//writer.WriteBcTime(bctime.ReturnGcTime());
	//cout << "bctimeseries writed\n";
	//writer.WriteBcsluice(sluice.ReturnBcSluice());
	//cout << "bcsluices writed\n";
	//writer.WriteSluice(sluice.ReturnSluice());
	//cout << "sluices writed\n";
	//writer.WriteBcWeir(weir.ReturnBcWeir());
	//cout << "bcweirs writed\n";
	//writer.WriteWeir(weir.ReturnWeir());
	//cout << "weirs writed\n";
	//writer.WritePump(pump.ReturnGrPump());
	//cout << "pump writed\n";
	//writer.WritePond(pond.ReturnPond());
	//cout << "pond writed\n";


	cout << "123" << endl;
	return 0;
}

#endif



/*--- CheckFileExist函数 ------------------------------------*/
bool CheckFileExist(string filepath)
{
	File file(filepath);

	if (file.isDirectory()) return FALSE;
	else if (file.exists())	return TRUE;
	else                    return FALSE;
}



/*--- CheckFolderExist函数 ----------------------------------*/
bool CheckFolderExist(string filepath)
{
	File file(filepath);

	if (file.isDirectory())	return TRUE;
	else                    return FALSE;
}



/*--- AbsolutePath函数 --------------------------------------*/
string AbsolutePath(const string &_file)
{
	string file = _file;
	Path path(file);

	if (CheckFileExist(file))
	{
		file = path.toString(); 
	}
	else if (CheckFolderExist(file))
	{
		file = path.toString() + "\\log.txt"; 
	}
	else
	{
		path = Path::current(); 
		file = path.toString() + "\\log.txt"; 
	}

	return file;
}



/*--- SystemTime函数 ----------------------------------------*/
string SystemTime()
{
	SYSTEMTIME time;               
	GetLocalTime(&time);

	char ctime[50];
	sprintf(ctime, "%4d-%02d-%02d, %02d:%02d:%02d",
		    time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	return string(ctime);
}



/*--- log文件类 ---------------------------------------------*/
int Log::m_count = 0;
string Log::m_logfile; 
unique_ptr<Log> Log::m_log;


Log::Log(const string &logfile)
{
	string _logfile = logfile;
	m_logfile = AbsolutePath(_logfile); 

	FileOutputStream logstream(m_logfile, ios::out |ios::trunc);
	logstream << "\r\n\r\n " << INFO1 << ", " << SystemTime() << "\r\n";
}


Log::Log()
{
	Path path = Path::current();
	m_logfile = path.toString() + "\\log.txt";

	FileOutputStream logstream(m_logfile, ios::out | ios::trunc);
	logstream << "\r\n\r\n " << INFO1 << ", " << SystemTime() << "\r\n";
}


Log* Log::GetLog(string logfile)
{
	if (m_log.get() == nullptr)
	{
		if (logfile == "") m_log.reset(new Log());
		else               m_log.reset(new Log(logfile));
	}

	return m_log.get();
}


void Log::WriteLogFile(const string &text)
{
	FileOutputStream logstream(m_logfile, ios::app); 
	m_count++;

	logstream << "\r\n " ;
	logstream <<setw(5)<< m_count << "> " << SystemTime() << ", ";
	logstream << text <<"\r\n";
}


string Log::ReturnLogFile()const
{
	return string(m_logfile);
}



/*--- swmminp文件类 -----------------------------------------*/
string SwmmInp::m_inpfile;
unique_ptr<SwmmInp> SwmmInp::m_swmminp;


SwmmInp::SwmmInp()
{
	Path path = Path::current();
	string file = path.toString() + "\\swmm.inp";

	if (CheckFileExist(file))
	{
		m_inpfile = file;
		Log::GetLog()->WriteLogFile(format(INFO2,m_inpfile));
	}
	else
	{
		throw FileExistsException();
		Log::GetLog()->WriteLogFile(ERR1);
	}
}


SwmmInp::SwmmInp(const string &inpfile)
{
	if (CheckFileExist(inpfile))
	{
		m_inpfile = inpfile;
		Log::GetLog()->WriteLogFile(format(INFO2,m_inpfile));
	}
	else
	{
		throw FileExistsException();
		Log::GetLog()->WriteLogFile(ERR1);
	}
}


SwmmInp* SwmmInp::GetSwmmInp(string inpfile)
{
	if (m_swmminp.get() == nullptr)
	{
		if (inpfile == "") m_swmminp.reset(new SwmmInp());
		else               m_swmminp.reset(new SwmmInp(inpfile));
	}

	return m_swmminp.get();
}


string SwmmInp::ReturnInpFile()const
{
	return m_inpfile;
}



/*--- swmmnode类 --------------------------------------------*/
SwmmNode::SwmmNode(SwmmInp* swmm):m_inpfile(swmm->ReturnInpFile())
{
	ReadAllNode();
	ReadInflownode();
	ReadCoordinate();
}


void SwmmNode::ReadAllNode()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		NODE node;
		string type = read.getString(0);
		if      (type == "[JUNCTIONS]")    node.type = JUNCTION;
		else if (type == "[OUTFALLS]")     node.type = OUTFALL;
		else if (type == "[DIVIDERS]")     node.type = DIVIDER;
		else if (type == "[STORAGE]")      node.type = STORAGE;
		else continue;

		do 
		{
			read.nextLine();
		} while (read.getString(0)[0] == ';');
		
		while (read.getDataCount() > 0)
		{
			node.ID = read.getString(0);
			node.z = read.getDouble(1);

			if (node.type == JUNCTION)
			{
				node.initdepth = read.getDouble(3);
			}
			else
			{
				node.initdepth = 0;
				m_specnode.push_back(node);
			}

			m_node.push_back(node);
			read.nextLine();
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO6, int(m_node.size())));
	Log::GetLog()->WriteLogFile(INFO3, m_specnode);
}


void SwmmNode::ReadInflownode()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;
		if (read.getString(0) != "[INFLOWS]") continue;

		do
		{
			read.nextLine();
		} while (read.getString(0)[0] == ';');

		while (read.getDataCount() > 0)
		{
			NODE node;
			node.ID = read.getString(0);
			size_t index = FindIndex(node.ID, m_node);
			m_node[index].inflow = read.getString(2);

			read.nextLine();
		}
	}
}


void SwmmNode::ReadCoordinate()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;
		if (read.getString(0) != "[COORDINATES]") continue;

		do 
		{
			read.nextLine();
		} while (read.getString(0)[0] == ';');

		while (read.getDataCount() > 0)
		{
			string id = read.getString(0);
			size_t it = FindIndex(id, m_node);

			m_node[it].x = read.getDouble(1);
			m_node[it].y = read.getDouble(2);

			read.nextLine();
		}
	}
}


const vector<NODE>& SwmmNode::ReturnSwmmNode()const
{
	return m_node;
}



/*--- swmmpipe类 --------------------------------------------*/
SwmmPipe::SwmmPipe(SwmmInp* swmm) :m_inpfile(swmm->ReturnInpFile())
{
	ReadAllPipe();
	ReadXSection();
}


void SwmmPipe::ReadAllPipe()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		PIPE pipe;
		string type = read.getString(0);
		if      (type == "[CONDUITS]")   pipe.ptype = CONDUIT;
		else if (type == "[PUMPS]")      pipe.ptype = PUMP;
		else if (type == "[ORIFICES]")   pipe.ptype = ORIFICE;
		else if (type == "[WEIRS]")      pipe.ptype = WEIR;
		else if (type == "[OUTLETS]")    pipe.ptype = OUTLET;
		else continue;

		do {
			read.nextLine();
		} while (read.getString(0)[0] == ';');

		while (read.getDataCount() > 0)
		{
			pipe.ID       = read.getString(0);
			pipe.fromnode = read.getString(1);
			pipe.tonode   = read.getString(2);
			
			if (pipe.ptype == CONDUIT)
			{
				pipe.initflow = read.getDouble(7);
			}
			else
			{
				m_specpipe.push_back(pipe);
			}

			m_pipe.push_back(pipe);
			read.nextLine();
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO7, int(m_pipe.size())));
}


void SwmmPipe::ReadXSection()
/*---------------------------
对于非IRREGULAR管段，将转为IRREGULAR断面，
命名为管段名+“_sec”。
-----------------------------*/
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;
		if (read.getString(0) != "[XSECTIONS]") continue;

		do
		{
			read.nextLine();
		} while (read.getString(0)[0] == ';');

		while (read.getDataCount() > 0)
		{
			string id = read.getString(0);
			size_t it = FindIndex(id, m_pipe);

			string type = read.getString(1);
			if      (type == "IRREGULAR")   m_pipe[it].ttype = IRREGULAR;
			else if (type == "RECT_OPEN")   m_pipe[it].ttype = RECT_OPEN;
			else if (type == "TRAPEZOIDAL") m_pipe[it].ttype = TRAPEZOIDAL;
			else if (type == "TRIANGULAR")  m_pipe[it].ttype = TRIANGULAR;
			else m_pipe[it].ttype = OTHER;

			switch (m_pipe[it].ttype)
			{
			case IRREGULAR: 
				m_pipe[it].shape = read.getString(2);break;
			case OTHER:
				m_pipe[it].shape = ""; 
				m_specsect.push_back(m_pipe[it]);
				Log::GetLog()->WriteLogFile(format(ERR4, id));break;
			default:
				m_pipe[it].shape = id + "_sec";
				m_specsect.push_back(m_pipe[it]);break;
			}
						
			read.nextLine();
		}
	}

	Log::GetLog()->WriteLogFile(INFO4, m_specpipe);
	Log::GetLog()->WriteLogFile(WARN2, m_specpipe);
}


const vector<PIPE>& SwmmPipe::ReturnSwmmPipe()const
{
	return m_pipe;
}



/*--- swmmtime类 --------------------------------------------*/
SwmmTime::SwmmTime(SwmmInp* swmm) :m_inpfile(swmm->ReturnInpFile())
{
	 ReadSwmmTime(); 
}


void SwmmTime::ReadSwmmTime()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;
		if (read.getString(0) != "[TIMESERIES]") continue;

		do
		{
			read.nextLine();
		} while (read.getString(0)[0] == ';');

		while (read.getDataCount() > 0)
		{
			if (read.getString(0)[0] == ';')
			{
				read.nextLine(); continue;
			}
			else if (read.getDataCount() >= 4)
			{
				Log::GetLog()->WriteLogFile(format(WARN1, read.getString(0)));

				while (read.getDataCount() == 4)
				{
					read.nextLine();
				}	continue;
			}

			TIME serie;
			serie.ID = read.getString(0);
			while (read.getDataCount() == 3)
			{
				XY xy;
				try 
				{
					xy.x = read.getDouble(1);
					xy.y = read.getDouble(2);
				}
				catch (...) 
				{
					Log::GetLog()->WriteLogFile(format(WARN1, serie.ID));
					while (read.getDataCount() == 3)
					{
						read.nextLine();
					}	continue;
				}

				serie.value.push_back(xy);
				read.nextLine();
			}

			if (serie.value.size())
			{
				m_time.push_back(serie);
				continue;
			}
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO8, int(m_time.size())));
}


const vector<TIME>& SwmmTime::ReturnSwmmTime()const
{
	return m_time;
}



/*--- swmmtransect类 ----------------------------------------*/
SwmmTransect::SwmmTransect(SwmmInp* swmm) :m_inpfile(swmm->ReturnInpFile())
{
	ReadSwmmNodePipe(); cout << "node\n";
	ReadSwmmTransect(); cout << "tran\n";
	GetUnirregularTransect(); cout<< "irregular\n";
	
	Log::GetLog()->WriteLogFile(WARN3, m_spectran);
}


void SwmmTransect::ReadSwmmNodePipe()
/*----------------------------------
读取所有节点的名称（ID），高程（num[0]）;
读取所有管道的名称（ID），起点终点（str[0],str[1]）,糙率（num[0]）。
------------------------------------*/
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		DATA data;
		string datatype = read.getString(0);

		if (datatype == "[JUNCTIONS]" || datatype == "[OUTFALLS]" ||
			datatype == "[DIVIDERS]"  || datatype == "[STORAGE]"   )
		{
			do
			{
				read.nextLine();
			} while (read.getString(0)[0] == ';');

			while (read.getDataCount() > 0)
			{
				data.ID = read.getString(0);
				data.num.push_back(read.getDouble(1));

				m_node.push_back(data);
				read.nextLine();
			}
		}

		if (datatype == "[CONDUITS]")
		{
			do
			{
				read.nextLine();
			} while (read.getString(0)[0] == ';');

			while (read.getDataCount() > 0)
			{
				data.ID = read.getString(0);
				data.str.push_back(read.getString(1));
				data.str.push_back(read.getString(2));
				data.num.push_back(read.getDouble(4));

				m_pipe.push_back(data);
				read.nextLine();
			}
		}

		if (datatype == "[ORIFICES]" || datatype == "[PUMPS]" ||
			datatype == "[WEIRS]"    || datatype == "[OUTLETS]")
		{
			do
			{
				read.nextLine();
			} while (read.getString(0)[0] == ';');

			while (read.getDataCount() > 0)
			{
				data.ID = read.getString(0);
				data.str.push_back(read.getString(1));
				data.str.push_back(read.getString(2));

				data.num.push_back(0.02);

				m_pipe.push_back(data);
				read.nextLine();
			}
		}
	}
}


void SwmmTransect::ReadSwmmTransect()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);
	
	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;
		
		if (read.getString(0) == "[TRANSECTS]")
		{
			do
			{
				read.nextLine();
			} while (read.getString(0)[0] == ';');

			while (read.getString(0) == "NC")
			{
				TRANSECT tran;
				XY xy;

				tran.leftrough = read.getDouble(1);
				tran.rightrough = read.getDouble(2);
				tran.channelrough = read.getDouble(3);
				read.nextLine();

				tran.ID = read.getString(1);
				tran.nstation = read.getInteger(2);
				tran.leftstation = read.getDouble(3);
				tran.rightstation = read.getDouble(4);
				tran.wfactor = read.getDouble(8);
				tran.eoffset = read.getDouble(9);
				read.nextLine();

				while (read.getString(0) == "GR")
				{
					for (int j = 1; j <= 5; j++)
					{
						if (tran.stations.size() >= tran.nstation) break;

						xy.x = read.getDouble(2 * j);
						xy.y = read.getDouble(2 * j - 1);

						tran.stations.push_back(xy);
					}

					read.nextLine();
					if (read.getDataCount() < 1) break;
				}

				m_transect.push_back(tran);

				read.nextLine();
				if (read.getDataCount() < 1) break;
			}
		}
	}
}


void SwmmTransect::GetUnirregularTransect()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;
		if (read.getString(0) != "[XSECTIONS]") continue;

		do
		{
			read.nextLine();
		} while (read.getString(0)[0] == ';');

		while (read.getDataCount() > 0)
		{
			TRANSECT tran;
			string conduit = read.getString(0);
			string type = read.getString(1);
			if      (type == "RECT_OPEN")   tran = RectopenTransect(read,conduit);
			else if (type == "TRAPEZOIDAL") tran = TrapezoidalTransect(read,conduit);
			else if (type == "TRIANGULAR")  tran = TriangularTransect(read,conduit);
			else	{read.nextLine();		continue;}

			m_spectran.push_back(tran);
			m_transect.push_back(tran);
			read.nextLine();
		}
	}
}


const vector<TRANSECT>& SwmmTransect::ReaturnSwmmTransect()const
{
	return m_transect;
}


TRANSECT SwmmTransect::RectopenTransect(DataStreamReader &read,string conduit)
/*----------------------------------------------------------------------------
将管道敞开矩形断面转为IRREGULAR断面，命名规则为管道名+"_sec"；
断面点数为5，断面无左右岸点，断面糙率为管道糙率，断面高程基于管道起点高程。
------------------------------------------------------------------------------*/
{
	size_t pindex = FindIndex(conduit, m_pipe);
	DATA   pipe = m_pipe[pindex];

	size_t nindex = FindIndex(pipe.str[0], m_node);
	DATA   node = m_node[nindex];
	
	TRANSECT tran;
	tran.ID = conduit + "_sec";
	tran.nstation = 5;
	tran.wfactor = 0;
	tran.eoffset = node.num[0];
	tran.leftstation = tran.rightstation = 0.0;
	tran.leftrough = tran.rightrough = tran.channelrough = pipe.num[0];

	double height = read.getDouble(2);
	double width  = read.getDouble(3);

	vector<XY> stations(5);
	stations[0].x = height, stations[0].y = 0;
	stations[1].x = 0,      stations[1].y = 0;
	stations[2].x = 0,      stations[2].y = width / 2;
	stations[3].x = 0,      stations[3].y = width;
	stations[4].x = height, stations[4].y = width;
	tran.stations = stations;

	return tran;
}


TRANSECT SwmmTransect::TrapezoidalTransect(DataStreamReader &read, string conduit)
/*--------------------------------------------------------------------------------
将管道敞开倒梯形断面转为IRREGULAR断面，命名规则为管道名+"_sec"；
swmm不允许正梯形断面；swmm梯形断面边坡为"水平/垂直"，即反坡度。
----------------------------------------------------------------------------------*/
{
	size_t pindex = FindIndex(conduit, m_pipe);
	DATA   pipe = m_pipe[pindex];

	size_t nindex = FindIndex(pipe.str[0], m_node);
	DATA   node = m_node[nindex];

	TRANSECT tran;
	tran.ID = conduit + "_sec";
	tran.nstation = 5;
	tran.wfactor = 0;
	tran.eoffset = node.num[0];
	tran.leftstation = tran.rightstation = 0.0;
	tran.leftrough = tran.rightrough = tran.channelrough = pipe.num[0];

	double height = read.getDouble(2);
	double width  = read.getDouble(3);
	double lslope = read.getDouble(4);
	double rslope = read.getDouble(5);

	vector<XY> stations(5);
	stations[0].x = height, stations[0].y = 0;
	stations[1].x = 0,      stations[1].y = height * lslope;
	stations[2].x = 0,      stations[2].y = height * lslope + width / 2;
	stations[3].x = 0,      stations[3].y = height * lslope + width;
	stations[4].x = height, stations[4].y = height * lslope + width + height * rslope;
	tran.stations = stations;

	return tran;
}


TRANSECT SwmmTransect::TriangularTransect(DataStreamReader &read, string conduit)
/*----------------------------------------------------------------------------
将管道敞开倒三角断面转为IRREGULAR断面，命名规则为管道名+"_sec"；
断面点数为5，断面无左右岸点，断面糙率为管道糙率，断面高程基于管道起点高程。
------------------------------------------------------------------------------*/
{
	size_t pindex = FindIndex(conduit, m_pipe);
	DATA   pipe = m_pipe[pindex];

	size_t nindex = FindIndex(pipe.str[0], m_node);
	DATA   node = m_node[nindex];

	TRANSECT tran;
	tran.ID = conduit + "_sec";
	tran.nstation = 5;
	tran.wfactor = 0;
	tran.eoffset = node.num[0];
	tran.leftstation = tran.rightstation = 0.0;
	tran.leftrough = tran.rightrough = tran.channelrough = pipe.num[0];

	double height = read.getDouble(2);
	double width = read.getDouble(3);

	vector<XY> stations(5);
	stations[0].x = height,   stations[0].y = 0;
	stations[1].x = height/2, stations[1].y = width/4;
	stations[2].x = 0,        stations[2].y = width/2;
	stations[3].x = height/2, stations[3].y = 3*width/4;
	stations[4].x = height,   stations[4].y = width;
	tran.stations = stations;

	return tran;
}



/*--- swmmoption类 ------------------------------------------*/
SwmmOption::SwmmOption(SwmmInp* swmm) :m_inpfile(swmm->ReturnInpFile())
{
	ReadSwmmOption();
	Log::GetLog()->WriteLogFile(INFO10);
}


void SwmmOption::ReadSwmmOption()
{
	FileInputStream fis(m_inpfile);
	DataStreamReader read(fis, m_inpfile);

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		if (read.getString(0) == "START_DATE")
		{
			m_option.startdate = read.getString(1); continue;
		}

		if (read.getString(0) == "START_TIME")
		{
			m_option.starttime = read.getString(1); continue;
		}

		if (read.getString(0) == "END_DATE")
		{
			m_option.enddate = read.getString(1); continue;
		}

		if (read.getString(0) == "END_TIME")
		{
			m_option.endtime = read.getString(1); continue;
		}

		if (read.getString(0) == "REPORT_STEP")
		{
			m_option.rptstep = read.getString(1); continue;
		}

		if (read.getString(0) == "ROUTING_STEP")
		{
			m_option.routstep = read.getString(1); continue;
		}

		if (read.getString(0) == "HEAD_TOLERANCE")
		{
			m_option.headtol = read.getDouble(1); continue;
		}

		if (read.getString(0) == "THREADS")
		{
			m_option.thread = read.getInteger(1); continue;
		}

		if (read.getString(0) == "MAX_TRIALS")
		{
			m_option.maxtrial = read.getInteger(1); continue;
		}
	}
}


const OPTION& SwmmOption::ReaturnSwmmOption()const
{
	return m_option;
}



/*--- setnodesite函数 ---------------------------------------*/
const vector<NODESITE> SwmmToGrms::SetNodeSite(const vector<NODE>& node, const vector<PIPE>& pipe)
{
	vector<NODESITE> nodesite;
	vector<string> fromnode, tonode;

	vector<PIPE>::const_iterator piter;
	for (piter = pipe.begin(); piter != pipe.end(); piter++)
	{
		fromnode.push_back((*piter).fromnode);
		tonode.push_back((*piter).tonode);
	}

	vector<NODE>::const_iterator niter;
	for (niter = node.begin(); niter != node.end(); niter++)
	{
		NODESITE site;
		site.ID = (*niter).ID;

		int nfrom  = (int)count(fromnode.begin(), fromnode.end(), site.ID);
		int nto    = (int)count(tonode.begin(), tonode.end(), site.ID);
		int ntotal = nfrom + nto;
		site.count = ntotal;
		site.num = ntotal;

		switch (ntotal)
		{
		case 2: site.site  = Inner; break;
		case 1: site.site  = (nfrom == 1 ? Start : End); break;
		case 0: site.site  = Out; break;
		default: site.site = Cross;
		}

		if ((*niter).type == STORAGE) site.site = Cross;

		nodesite.push_back(site);
	}

	return nodesite;
}



/*--- ResetSerialNumber函数 ---------------------------------*/
inline string ResetSerialNumber(string name, int num)
{
	if (num < 1 or num>10)
	{
		Log::GetLog()->WriteLogFile(format(ERR5, name));
		throw DataException(format(ERR5, name));
	}

	char suffix = 'a' + num - 1;
	return (name + "_" + suffix);
}



/*--- junctionconvertor类 -----------------------------------*/
JunctionConvertor::JunctionConvertor(const vector<NODE>& node, const vector<NODESITE>& site,
	const vector<TIME>& time) :m_node(node), m_nodesite(site), m_time(time)
{
	GenerateJunction();
	GeneratePsource();
	GenerateLocation();
	GenerateBcSection();
}


void JunctionConvertor::GenerateJunction()
{
	int count = 1;

	vector<NODESITE>::const_iterator iter;
	for (iter = m_nodesite.begin(); iter != m_nodesite.end(); iter++)
	{
		if ((*iter).site != Cross) continue;

		grJUNCTION temp;
		temp.ID = "CROSS" + std::to_string(count);

		for (int i = 1; i <= (*iter).count; i++)
		{
			string dmid = ResetSerialNumber((*iter).ID, i);
			temp.dmid.push_back(dmid);
		}

		temp.size = (int)temp.dmid.size();

		count++;
		m_junc.push_back(temp);
	}

	Log::GetLog()->WriteLogFile(format(INFO12, count-1));
}


void JunctionConvertor::GeneratePsource()
{
	int count = 1;

	vector<NODE>::const_iterator iter;
	for (iter = m_node.begin(); iter != m_node.end(); iter++)
	{
		if ((*iter).inflow != "")
		{
			PSOURCE tmp;
			tmp.ID = format("ps%d",count);
			tmp.dm = (*iter).ID;

			size_t index1 = FindIndex((*iter).inflow, m_time);
			tmp.serie = m_time[index1].value;
			tmp.length = tmp.serie.size();

			size_t index2 = FindIndex(tmp.dm, m_nodesite);
			if (m_nodesite[index2].site == Cross)
			{
				Log::GetLog()->WriteLogFile(format(WARN14, tmp.ID));
			}

			count++;
			m_psource.push_back(tmp);
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO28, (int)m_psource.size()));
}


void JunctionConvertor::GenerateLocation()
{
	vector<NODESITE>::const_iterator iter;
	for (iter = m_nodesite.begin(); iter != m_nodesite.end(); iter++)
	{
		int    index = FindIndex((*iter).ID, m_node);
		string nodeid = (*iter).ID;


		if ((*iter).site != Cross)
		{
			LOCATION coor;

			coor.ID = nodeid;
			coor.x = m_node[index].x;
			coor.y = m_node[index].y;

			m_loca.push_back(coor);
		}
		else
		{
			LOCATION coor;
			int count = (*iter).count;

			for (int i = 1; i <= count; i++)
			{
				coor.ID = ResetSerialNumber(nodeid, i);
				coor.x = m_node[index].x;
				coor.y = m_node[index].y;

				m_loca.push_back(coor);
			}
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO14, (int)m_loca.size()));
}


void JunctionConvertor::GenerateBcSection()
{
	int ID = 1;
	vector<NODESITE>::const_iterator iter;
	for (iter = m_nodesite.begin(); iter != m_nodesite.end(); iter++)
	{
		if ((*iter).site == Start || (*iter).site == End)
		{
			bcSECTION temp;
			temp.ID = ID; 
			temp.dm = (*iter).ID;
			ID++;

			m_bcsect.push_back(temp);
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO13, (int)m_bcsect.size()));
}


const vector<grJUNCTION>&JunctionConvertor::ReturnGrJunction()const
{
	return m_junc;
}


const vector<LOCATION>&  JunctionConvertor::ReturnLocation()const
{
	return m_loca;
}


const vector<bcSECTION>& JunctionConvertor::ReturnBcSection()const
{
	return m_bcsect;
}


const vector<PSOURCE>&   JunctionConvertor::ReturnPsource()const
{
	return m_psource;
}



/*--- hdconvertor类 -----------------------------------------*/
HdConvertor::HdConvertor(const vector<TRANSECT> &tran, const vector<PIPE> &pipe,
	const vector<NODE> &node, const vector<NODESITE> &site) :
	m_tran(tran), m_pipe(pipe), m_node(node), m_nodesite(site), m_hd(),m_temphd()
{
	MatchSpecPipeDm(); 
	GenerateDm();     
	GenerateTempHd(); 
	GenerateHd(); 
	GenerateManing();
}


void HdConvertor::MatchSpecPipeDm()
{
	vector<PIPE>::iterator iter;
	for (iter = m_pipe.begin(); iter != m_pipe.end(); iter++)
	{
		if ((*iter).shape == "")
		{
			string upnode, tonode;
			upnode = (*iter).fromnode;
			tonode = (*iter).tonode;

			size_t up, to;
			up = FindIndex(upnode, m_nodesite);
			to = FindIndex(tonode, m_nodesite);

			string node = upnode;
			if (m_nodesite[up].site == End || m_nodesite[up].site == Start)
			{
				if (m_nodesite[to].site == End || m_nodesite[to].site == Start)
				{
					Log::GetLog()->WriteLogFile(format(ERR6, (*iter).ID));
					continue;
				}
				else node = tonode;
			}

			string pipe = (*iter).ID;
			vector<PIPE>::iterator temp;

			temp = find_if(m_pipe.begin(), m_pipe.end(), [node, pipe](PIPE p)
				   ->bool {if ((p.ID != pipe && p.ptype != PUMP && p.ttype != OTHER) &&
				               (p.fromnode == node || p.tonode == node)) return true; 
				           else return false; });

			if (temp != m_pipe.end())
			{
				(*iter).shape = (*temp).shape;
				continue;
			}
			else
			{
				Log::GetLog()->WriteLogFile(format(ERR6, (*iter).ID));
				continue;
			}
		}
	}
}


void HdConvertor::GenerateDm()
{
	vector<PIPE>::const_iterator iter;
	for (iter = m_pipe.begin(); iter != m_pipe.end(); iter++)
	{
		string upnode, tonode;
		upnode = (*iter).fromnode;
		tonode = (*iter).tonode;

		size_t up, to;
		up = FindIndex(upnode, m_nodesite);
		to = FindIndex(tonode, m_nodesite);

		size_t tran = FindIndex((*iter).shape, m_tran);
		vector<XY> newtran = m_tran[tran].stations;
		if (!m_nodesite[up].useflag || !m_nodesite[to].useflag)
		{
			if (m_tran[tran].eoffset)
			{
				vector<XY>::iterator it; 
				for (it = newtran.begin(); it != newtran.end(); it++)
				{
					if(m_tran[tran].wfactor)
						(*it).x = (*it).x * m_tran[tran].wfactor;

					(*it).y += m_tran[tran].eoffset;
				}
			}
		}
		
		if (!m_nodesite[up].useflag)
		{
			DM temp;
			temp.ID = upnode;
			temp.tran = tran;
			temp.stations = newtran;
			temp.size = (int)temp.stations.size();

			m_nodesite[up].useflag = TRUE;
			m_dm.push_back(temp);
		}

		if (!m_nodesite[to].useflag)
		{
			DM temp;
			temp.ID = tonode;
			temp.tran = tran;
			temp.stations = newtran;
			temp.size = (int)temp.stations.size();

			m_nodesite[to].useflag = TRUE;
			m_dm.push_back(temp);
		}
	}
}


void HdConvertor::GenerateTempHd()
{
	vector<NODESITE>::const_iterator iter;
	for (iter = m_nodesite.begin(); iter != m_nodesite.end(); iter++)
	{
		if ((*iter).site == Start || (*iter).site == Cross)
		{
			do
			{
				TEMPHD temp;
				temp.ID = "reach";

				SearchEachReach(temp, (*iter).ID);

				if (temp.pipes.size())	m_temphd.push_back(temp);

			} while ((*iter).count);
		}
		else continue;
	}
}


string HdConvertor::SearchEachReach(TEMPHD &temp, string id)
{
	size_t index1 = FindIndex(id, m_nodesite);
	NODESITE &site = m_nodesite[index1];

	temp.nodes.push_back(id);

	if (site.count <= 0)   return site.ID;
	if (site.site == End   && temp.pipes.size()) return site.ID;
	if (site.site == Cross && temp.pipes.size()) return site.ID;
	if (site.site == Start && temp.pipes.size()) return site.ID;

	vector<PIPE>::iterator iter;
	for (iter = m_pipe.begin(); iter != m_pipe.end(); iter++)
	{
		if ((*iter).useflag) continue;

		string node;
		if (site.ID == (*iter).fromnode)    node = (*iter).tonode;
		else if (site.ID == (*iter).tonode) node = (*iter).fromnode;
		else continue;
		
		temp.pipes.push_back((*iter).ID);
		(*iter).useflag = TRUE;

		size_t index2 = FindIndex(node, m_nodesite);
		site.count--;
		m_nodesite[index2].count--;
		
		return SearchEachReach(temp, node);
	}

	return site.ID;
}

 
void HdConvertor::GenerateHd()
{
	int Rnum = 1;
	vector<TEMPHD>::const_iterator thd;
	for (thd = m_temphd.begin(); thd != m_temphd.end(); thd++,Rnum++)
	{
		HD temp;
		temp.ID = format("reach%d", Rnum);
		temp.size = (*thd).nodes.size();

		vector<string>::const_iterator obj;
		for (obj = (*thd).nodes.begin(); obj != (*thd).nodes.end(); obj++)
		{
			string node = *obj;
			size_t index = FindIndex(node, m_nodesite);

			vector<DM>::iterator dm;
			dm = find_if(m_dm.begin(), m_dm.end(), 
				         [node](DM d) {return d.ID == node; });

			if (dm != m_dm.end())
			{				
				DM d = *dm;
				if (m_nodesite[index].site == Cross)
				{
					d.ID = ResetSerialNumber(node, m_nodesite[index].num);
					m_nodesite[index].num--;
				}
				m_dm.push_back(d);
				temp.dm.push_back(d);
			}
			else
			{
				Log::GetLog()->WriteLogFile(format(ERR7, node));
				continue;
			}
		}

		m_hd.push_back(temp);
	}

	Log::GetLog()->WriteLogFile(format(INFO15, (int)m_hd.size()));
}


void HdConvertor::GenerateManing()
{
	vector<HD>::const_iterator iter;
	for (iter = m_hd.begin(); iter != m_hd.end(); iter++)
	{
		const vector<DM> &DMs = (*iter).dm;

		vector<DM>::const_iterator it; 
		for (it = DMs.begin(); it != DMs.end(); it++)
		{
			size_t dmIndex = FindIndex((*it).ID, m_dm);
			
			SINGLE single ; 
			single.ID = (*it).ID;
			GenerateSingle(single, dmIndex);
			m_single.push_back(single);
		}
	}
	Log::GetLog()->WriteLogFile(format(INFO16, (int)m_single.size()));
}


void HdConvertor::GenerateSingle(SINGLE &single, size_t dm)
{
	double leftX, rightX;
	double leftR, rightR, chanelR;

	size_t tran = m_dm[dm].tran;

	leftX   = m_tran[tran].leftstation;
	rightX  = m_tran[tran].rightstation;
	leftR   = m_tran[tran].leftrough;
	rightR  = m_tran[tran].rightrough;
	chanelR = m_tran[tran].channelrough;

	double leftY = -1, rightY = -1;
	const vector<XY> &stations = m_tran[tran].stations;
	if (leftX > 0.0)
	{
		leftY = (*find_if(stations.begin(), stations.end(),
			              [leftX](XY xy) {return xy.x == leftX; })).y;
	}
	if (rightX > 0.0 && rightX < stations.back().x)
	{
		rightY = (*find_if(stations.begin(), stations.end(),
			               [rightX](XY xy) {return xy.x == rightX; })).y;
	}

	XY tmp;
	double rough;
	if (leftY > 0.0)
	{
		tmp.x = leftX;
		tmp.y = leftY;
		rough = (leftR > 0.0 ? leftR : 0.0);
		Log::GetLog()->WriteLogFile(format(ERR10, m_tran[tran].ID));

		single.station.push_back(tmp);
		single.rough.push_back(rough);
		single.size++;
	}
	if (rightY > 0.0 )
	{
		tmp.x = rightX;
		tmp.y = rightY;
		rough = (rightR > 0.0 ? rightR : 0.0);
		Log::GetLog()->WriteLogFile(format(ERR10, m_tran[tran].ID));

		single.station.push_back(tmp);
		single.rough.push_back( rough);
		single.size++;
	}

	tmp = stations.back(); 
	single.station.push_back(tmp); 
	single.rough.push_back(chanelR); 
	single.size++;
}


const vector<HD>& HdConvertor::ReturnHd()const
{
	return m_hd;
}


const vector<SINGLE>& HdConvertor::ReturnManing()const
{
	return m_single;
}



/*--- timeconvertor类 -----------------------------------*/
TimeConvertor::TimeConvertor(const vector<TIME> &time) :m_time(time)
{
	GenerateTime();	
}


void TimeConvertor::GenerateTime()
{
	int length = CheckTime();

	vector<TIME>::const_iterator iter;
	for (iter = m_time.begin(); iter != m_time.end(); iter++)
	{
		bcTIME tmp;
		tmp.ID = (*iter).ID;
		tmp.size = length;

		vector<XY>::const_iterator first = (*iter).value.begin();
		vector<XY>::const_iterator last = first + length;
		vector<XY> cut_value(first, last);
		tmp.value = cut_value;

		m_bctime.push_back(tmp);
	}

	Log::GetLog()->WriteLogFile(format(INFO18, (int)m_bctime.size()));
}


int TimeConvertor::CheckTime()
{
	int max = 0;
	vector<TIME>::const_iterator iter;
	for (iter = m_time.begin(); iter != m_time.end(); iter++)
	{
		max = ((*iter).value.size() > max ? (*iter).value.size() : max);
	}

	int min = max;
	for (iter = m_time.begin(); iter != m_time.end(); iter++)
	{
		min = ((*iter).value.size() < min ? (*iter).value.size() : min);
	}

	if (min != max) Log::GetLog()->WriteLogFile(WARN6);
	else            Log::GetLog()->WriteLogFile(INFO17);

	return min;
}


const vector<bcTIME>& TimeConvertor::ReturnGcTime()const
{
	return m_bctime;
}



/*--- dstconvertor类 ------------------------------------*/
DstConvertor::DstConvertor(const vector<LOCATION> &loc, const vector<HD> &hd) :
	m_loc(loc), m_hd(hd)
{
	GenerateDst();
}


void DstConvertor::GenerateDst()
{
	vector<HD>::const_iterator iter;
	for (iter = m_hd.begin(); iter != m_hd.end(); iter++)
	{
		vector<DM>::const_iterator it = (*iter).dm.begin();
		size_t index1 = FindIndex((*it).ID, m_loc);
		size_t index2 = 0;

		DST tmp;
		tmp.ID = (*it).ID;
		tmp.dis = 0.0;
		m_dst.push_back(tmp);

		for (it = (*iter).dm.begin() + 1; it != (*iter).dm.end(); it++)
		{
			string node = (*it).ID;
			index2 = FindIndex(node, m_loc);

			LOCATION loc1 = m_loc[index1];
			LOCATION loc2 = m_loc[index2];

			tmp.ID = node;
			tmp.dis = pow(pow(loc1.x - loc2.x, 2) + pow(loc1.y - loc2.y, 2), 0.5);

			index1 = index2;
			m_dst.push_back(tmp);
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO19, (int)m_dst.size()));
}


const vector<DST>& DstConvertor::ReturnDst()const
{
	return m_dst;
}



/*--- setconvertor类 ------------------------------------*/
SetConvertor::SetConvertor(const OPTION &o, const vector<NODE> &n,
	const vector<PIPE> &p) :
	m_option(o), m_node(n), m_pipe(p)
{
	GenerateSet();
}


void SetConvertor::GenerateSet()
{
	m_set.dz_ic = GenerateDzic(); 
	m_set.q_ic = GenerateQic(); 

	vector<double> time = GenerateTime();
	m_set.dt = time[0];
	m_set.t0 = time[1];
	m_set.tfinal    = time[2];
	m_set.dt_report = time[3];

	m_set.threadsnum = (m_option.thread > 2 ? m_option.thread : 2);
	m_set.maxinter = m_option.maxtrial;
	m_set.epsz = m_option.headtol;	

	Log::GetLog()->WriteLogFile(INFO20);
}


double SetConvertor::GenerateDzic()
{
	double allinitdepth = 0;
	vector<NODE>::const_iterator iter;
	for (iter = m_node.begin(); iter != m_node.end(); iter++)
	{
		allinitdepth += (*iter).initdepth;
	}

	return allinitdepth / (int)m_node.size();
}


double SetConvertor::GenerateQic()
{
	double allinitflow = 0;
	vector<PIPE>::const_iterator iter;
	for (iter = m_pipe.begin(); iter != m_pipe.end(); iter++)
	{
		allinitflow += (*iter).initflow;
	}

	return allinitflow / (int)m_pipe.size();
}


vector<double> SetConvertor::GenerateTime()
{
	vector<double> result;
	vector<int> tmp,tmp2;

	tmp = SeparateTimeStr(m_option.routstep); 
	double dt = tmp[0] * 3600 + tmp[1] * 60 + tmp[2];
	result.push_back(dt);

	tmp = SeparateTimeStr(m_option.starttime); 
	double t0 = tmp[0] + tmp[1] / 60 ;
	result.push_back(t0);

	tmp = SeparateTimeStr(m_option.startdate,"/"); 
	double startday = tmp[1];
	tmp2 = SeparateTimeStr(m_option.enddate,"/"); 
	double endday = tmp2[1];

	if (tmp[0] != tmp2[0] || tmp[2] != tmp2[2])
		Log::GetLog()->WriteLogFile(ERR12);

	tmp = SeparateTimeStr(m_option.endtime);
	double tend = tmp[0] + tmp[1] / 60 ;
	double tfinal = (endday - startday) * 24 + (tend - t0);
	result.push_back(tfinal);

	tmp = SeparateTimeStr(m_option.rptstep); 
	double dt_rpt = tmp[0] * 3600 + tmp[1] * 60 + tmp[2];
	result.push_back(dt_rpt);

	return result;
}


vector<int> SetConvertor::SeparateTimeStr(const string str, const string delim)
{
	vector<string> result;

	int delim_len = delim.size();
	size_t lastIndex=0, index=-1;

	while ((index = str.find(delim, lastIndex)) != -1)
	{
		result.push_back(str.substr(lastIndex, index - lastIndex));
		lastIndex = index + delim_len;
	}

	string lastStr = str.substr(lastIndex);
	if (!lastStr.empty())
	{
		result.push_back(lastStr);
	}

	if (result.size() != 3)
	{
		result = vector<string>{ "0","0","0" };
		Log::GetLog()->WriteLogFile(format(ERR11, str));
	}

	int time1 = strtol(result[0].c_str(), NULL, 0);
	int time2 = strtol(result[1].c_str(), NULL, 0);
	int time3 = strtol(result[2].c_str(), NULL, 0);

	return vector<int>{time1, time2, time3};
}


const SET& SetConvertor::ReturnSet()const
{
	return m_set;
}



/*--- sluiceconvertor类 ------------------------------------*/
SluiceConvertor::SluiceConvertor(const vector<PIPE> &p, const vector<NODESITE> &site) :
	m_pipe(p), m_node(site)
{
	GenerateSluice();
}


void SluiceConvertor::GenerateSluice()
{
	vector<PIPE>::const_iterator it;
	for (it = m_pipe.begin(); it != m_pipe.end(); it++)
	{
		if ((*it).ptype == ORIFICE)
		{
			string upnode = (*it).fromnode;
			string tonode = (*it).tonode;

			size_t up = FindIndex(upnode, m_node);
			size_t to = FindIndex(tonode, m_node);

			if (m_node[up].count == 1 || m_node[to].count == 1)
			{
				bcSLUICE tmp;
				tmp.ID = upnode;
				tmp.dm = upnode;
				m_bcsluice.push_back(tmp);
			}
			else
			{
				SLUICE tmp;
				tmp.ID = upnode;
				tmp.dm = upnode;
				m_sluice.push_back(tmp);
			}
		}
	}

	Log::GetLog()->WriteLogFile(format(WARN7, (int)m_bcsluice.size()));
	Log::GetLog()->WriteLogFile(format(WARN8, (int)m_sluice.size()));
}


const vector<bcSLUICE>& SluiceConvertor::ReturnBcSluice()const
{
	return m_bcsluice;
}


const vector<SLUICE>& SluiceConvertor::ReturnSluice()const
{
	return m_sluice;
}



/*--- weirconvertor类 -----------------------------------*/
WeirConvertor::WeirConvertor(const vector<PIPE> &p, const vector<NODESITE> &site) :
	m_pipe(p), m_node(site)
{
	GenerateWeir();
}


void WeirConvertor::GenerateWeir()
{
	vector<PIPE>::const_iterator it;
	for (it = m_pipe.begin(); it != m_pipe.end(); it++)
	{
		if ((*it).ptype == WEIR)
		{
			string upnode = (*it).fromnode;
			string tonode = (*it).tonode;

			size_t up = FindIndex(upnode, m_node);
			size_t to = FindIndex(tonode, m_node);

			if (m_node[up].count == 1 || m_node[to].count == 1)
			{
				bcWEIR tmp;
				tmp.ID = upnode;
				tmp.dm = upnode;
				m_bcweir.push_back(tmp);
			}
			else
			{
				grWEIR tmp;
				tmp.ID = upnode;
				tmp.dm = upnode;
				m_weir.push_back(tmp);
			}
		}
	}

	Log::GetLog()->WriteLogFile(format(WARN9, (int)m_bcweir.size()));
	Log::GetLog()->WriteLogFile(format(WARN10, (int)m_weir.size()));
}


const vector<bcWEIR>& WeirConvertor::ReturnBcWeir()const
{
	return m_bcweir;
}


const vector<grWEIR>& WeirConvertor::ReturnWeir()const
{
	return m_weir;
}



/*--- pumpconvertor类 -----------------------------------*/
PumpConvertor::PumpConvertor(const vector<PIPE> &p,const vector<NODESITE> &n) :
	m_pipe(p),  m_node(n)
{
	GenerateGrPump();
}


void PumpConvertor::GenerateGrPump()
{
	vector<PIPE>::const_iterator it;
	for (it = m_pipe.begin(); it != m_pipe.end(); it++)
	{
		if ((*it).ptype == PUMP)
		{
			string upnode = (*it).fromnode;
			string tonode = (*it).tonode;

			size_t up = FindIndex(upnode, m_node);
			size_t to = FindIndex(tonode, m_node);

			if (m_node[up].site == Cross)
				Log::GetLog()->WriteLogFile(format(WARN12, (*it).ID, upnode));
			else if (m_node[to].site == Cross)
				Log::GetLog()->WriteLogFile(format(WARN12, (*it).ID, tonode));

			grPUMP tmp;
			tmp.ID = (*it).ID;
			tmp.fromdm = upnode;
			tmp.todm = tonode;
			m_pump.push_back(tmp);
		}
	}

	Log::GetLog()->WriteLogFile(format(WARN13, (int)m_pump.size()));
}


const vector<grPUMP>& PumpConvertor::ReturnGrPump()const
{
	return m_pump;
}



/*--- pondconvertor类 -----------------------------------*/
PondConvertor::PondConvertor(const vector<NODE> &n, const vector<NODESITE> &s, 
	const vector<grJUNCTION> &c) :	m_node(n), m_site(s),m_cross(c)
{
	GeneratePond();
}


void PondConvertor::GeneratePond()
{
	vector<NODE>::const_iterator it;
	for (it = m_node.begin(); it != m_node.end(); it++)
	{
		if ((*it).type == STORAGE)
		{
			size_t index = FindIndex((*it).ID, m_site);
			if (m_site[index].count <= 1) continue;

			POND tmp;
			tmp.ID = (*it).ID;			
			tmp.cross = FindGrJunction(tmp.ID);

			m_pond.push_back(tmp);
		}
	}

	Log::GetLog()->WriteLogFile(format(INFO21, (int)m_pond.size()));
}


string PondConvertor::FindGrJunction(string pond)
{
	string pond1 = pond + "_a";

	vector<grJUNCTION>::const_iterator it;
	for (it = m_cross.begin(); it != m_cross.end(); it++)
	{
		vector<string>::const_iterator str;
		str = find_if((*it).dmid.begin(), (*it).dmid.end(),
			[pond1](string s) {return s == pond1; });
				
		if (str != (*it).dmid.end()) return (*it).ID;
	}

	Log::GetLog()->WriteLogFile(format(ERR13, pond));
	return "";
}


const vector<POND>& PondConvertor::ReturnPond()const
{
	return m_pond;
}



/*--- grmswriter类 --------------------------------------*/
GrmsWriter::GrmsWriter()
{
	Path path = Path::current();
	
	GenerateFolder(path.toString());
}


GrmsWriter::GrmsWriter(const string grms)
{
	Path path(grms);
	GenerateFolder(path.toString());
}


void GrmsWriter::GenerateFolder(const string &path)
{
	File test(path);
	if (!test.isDirectory() || !test.exists())
	{
		Log::GetLog()->WriteLogFile(format(ERR14, path));
		throw Poco::PathNotFoundException(format(ERR14, path));
	}

	string structure = path+ "\\Structure";
	_mkdir(structure.c_str());
	m_structure = structure;

	string setting = path + "\\Setting";
	_mkdir(setting.c_str());
	m_setting = setting;

	string topo = path + "\\Topo";
	_mkdir(topo.c_str());
	m_topo = topo;

	string bc = path + "\\BC";
	_mkdir(bc.c_str());
	m_bc = bc;
}


string GrmsWriter::GenerateFile(const string &folder, const string &file)
{
	string path = folder + "\\" + file;
	FILE   *tmp = fopen(path.c_str(), "w");
	fclose(tmp);

	return path;
}


void GrmsWriter::WriteHd(const vector<HD>& hd)
{
	if (!hd.size()) return;

	string m_hd = GenerateFile(m_topo, "HD.inp");

	FileOutputStream file(m_hd);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	vector<HD>::const_iterator iter;
	for (iter = hd.begin(); iter != hd.end(); iter++)
	{
		file << endl;
		file <<"[reach]" <<setw(5) <<"  ***************************" <<(*iter).ID <<endl;
		file << (*iter).size << endl;
	
		vector<DM>::const_iterator it;
		for (it = (*iter).dm.begin(); it != (*iter).dm.end(); it++)
		{
			file << setw(14) << "";
			file << setw(14) << (*it).size << (*it).ID << endl;

			vector<XY>::const_iterator xy;
			for (xy = (*it).stations.begin(); xy != (*it).stations.end(); xy++)
			{
				file << setw(5) << "";
				file << setw(12) << (*xy).x << setw(12) << (*xy).y << endl;
			}
		}
	}

	Log::GetLog()->WriteLogFile(INFO22);
}


void GrmsWriter::WriteDst(const vector<DST>& dst)
{
	if (!dst.size()) return;

	string m_dst = GenerateFile(m_topo, "dst.inp");

	FileOutputStream file(m_dst);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	vector<DST>::const_iterator iter;
	for (iter = dst.begin(); iter != dst.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(15) << (*iter).dis << endl;
	}

	Log::GetLog()->WriteLogFile(INFO23);
}


void GrmsWriter::WriteJunction(const vector<grJUNCTION>& junc)
{
	if (!junc.size()) return;

	string m_jun = GenerateFile(m_topo, "Junction.inp");

	FileOutputStream file(m_jun);
	file.setf(ios::left);

	int counted = 0;
	find_if(junc.begin(), junc.end(), 
		[&counted](grJUNCTION j)->bool { counted += j.size; return FALSE; });

	file << "[JunctionsNum]  !汊点数   所有汊点的断面总数量" << endl;
	file << setw(15) << junc.size() << setw(15) << counted << endl<<endl;

	file << "[Junctions] !汊点ID  本汊点断面数量 断面ID  ..." << endl;
	vector<grJUNCTION>::const_iterator iter;
	for (iter = junc.begin(); iter != junc.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(10) << (*iter).size ;

		for (auto it : (*iter).dmid) file << setw(12) << it;

		file << endl;
	}

	Log::GetLog()->WriteLogFile(INFO24);
}


void GrmsWriter::WriteLocation(const vector<LOCATION>& loc)
{
	if (!loc.size()) return;

	string m_loc = GenerateFile(m_topo, "CrossSectionLocation.inp");

	FileOutputStream file(m_loc);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	vector<LOCATION>::const_iterator iter;
	for (iter = loc.begin(); iter != loc.end(); iter++)
	{
		file << setw(15) << (*iter).ID;
		file << setw(20) << (*iter).x << setw(20) << (*iter).y << endl;
	}

	Log::GetLog()->WriteLogFile(INFO25);
}


void GrmsWriter::WriteSeting(const SET &set)
{
	string m_set = GenerateFile(m_setting, "Setting.inp");

	FileOutputStream file(m_set);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	file << "! 变量名字长度小于20个字符" << endl;
	file << "!=========================================================================" << endl;
	file << "!****************************  联  解  模  式  ****************************" << endl;
	file << "!=========================================================================" << endl;
	file << "!设定联解方式：3 为三级联解；4 为四级联解； 41 为四级联解 + 压缩矩阵" << endl;
	file << "method = " << set.method << endl<<endl<<endl;

	file << "!=========================================================================" << endl;
	file << "!****************************  初  始  条  件  ****************************" << endl;
	file << "!=========================================================================" << endl;
	file << "! 模型初始化相关参数"     << endl;
	file << "!ic_options = 0: 冷启动，水深 dz_ic, 流量 q_ic "<<endl;
	file << "!ic_options = 1 : 冷启动，水位  z_ic, 流量 q_ic"<<endl;
	file << "!ic_options = 2 : 热启动" << endl;
	file << "ic_options = " << set.ic_options << endl << endl;
	file << "!热启动文件"   << endl;
	file << "ic_file = Restart.dat" << endl << endl;
	file << "!水深（位）（米）、流量（立方米 / 秒）" << endl;
	file << "dz_ic = "   << set.dz_ic << endl;
	file << "z_ic = 3.8" << endl;
	file << "q_ic = "    << set.q_ic << endl << endl<<endl;

	file << "!=========================================================================" << endl;
	file << "!****************************  结  果  输  出  ****************************" << endl;
	file << "!=========================================================================" << endl;
	file << "!输出文件名称起始编号" << endl;
	file << "outFileStartID = 0" << endl << endl;
	file << "!监测断面" << endl;
	file << "monitor_on = F" << endl << endl;
	file << "!输出tecplot二进制格式的动画" << endl;
	file << "outTecplot = T" << endl;
	file << "!每个断面重新采样点数为tecplotPointNum * 2 + 1" << endl;
	file << "tecplotPointNum = 9" << endl << endl<<endl;

	file << "!========================================================================= "<<endl;
	file << "!****************************  时  间  步  长  ****************************"<<endl;
	file << "!========================================================================= "<<endl;
	file << "!四点隐士差分格式的空间加权系数" << endl;
	file << "!0.5 <= gama <= 1  无条件稳定  " << endl;
	file << "!gama <= 0.5      条件稳定		" << endl;
	file << "!gama > 0.5       推荐		  " << endl;
	file << "gama = " << set.gama << endl << endl;
	file << "!时间步长（秒）" << endl;
	file << "dt = " << set.dt << endl << endl;
	file << "!模拟起始时间（小时）" << endl;
	file << "t0 = " << set.t0 << endl << endl;
	file << "!模拟终止时间（小时）" << endl;
	file << "tfinal = " << set.tfinal << endl << endl;
	file << "!结果输出时间间隔（秒）" << endl;
	file << "!dt_out 应该为 dt 的整数倍" << endl;
	file << "dt_out = " << set.dt_out << endl << endl;
	file << "!报告时间步长（秒）" << endl;
	file << "!dt_report 应该为 dt 的整数倍" << endl;
	file << "dt_report = " << set.dt_report << endl << endl;
	file << "!输出监测结果的时间步长（秒），monitor_on = T 时，有效" << endl;
	file << "!dt_monitor 应该为 dt 的整数倍" << endl;
	file << "dt_monitor = " << set.dt_monitor << endl << endl << endl;

	file << "!========================================================================= "<<endl;
	file << "!****************************  其  他  参  数  ****************************"<<endl;
	file << "!========================================================================= "<<endl;
	file << "!水力半径，1表示水力半径，0 表示阻力半径" << endl;
	file << "HydraulicRadiusType = 1" << endl << endl;
	file << "!汊点是否有蓄水能力" << endl;
	file << "Junction_Pound = F" << endl;
	file << "!线程数(共享内存并行模式)" << endl;
	file << "ThreadsNum = " << set.threadsnum << endl << endl;
	file << "!是否启动DM_DeadAreaMinWidth.dat对个别断面特殊处理（F: 不启动，T：启动）" << endl;
	file << "useUniformMinHydraulic = T" << endl<<endl;
	file << "!最小死水面积（平方米）" << endl;
	file << "deadArea = 0.0001" << endl<<endl;
	file << "!最小水面宽度（米）" << endl;
	file << "wsMin = 0.0000" << endl << endl;
	file << "!最小水深（米）" << endl;
	file << "RwMin = 0.0001" << endl;
	file << "!最大迭代次数" << endl;
	file << "maxIter = "<<set.maxinter << endl<<endl;
	file << "!迭代过程中水位、流量的最大误差" << endl;
	file << "epsZ = "<<set.epsz << endl;
	file << "epsQ = "<<set.epsq << endl<<endl;
	file << "!重力加速度（m / s ^ 2）" << endl;
	file << "!广州9.7883  北京9.8012  南京9.7944  福州9.7916  杭州9.7930  汉口9.7936  上海9.7940  南宁9.7876" << endl;
	file << "g = "<<set.g << endl<<endl;
	file << "!混合流区域，断面弗洛德数位于区间（Fr1，Fr2）时，为混合流" << endl;
	file << "Fr1 = " << set.fr1 << endl;
	file << "Fr2 = " << set.fr2 << endl << endl << endl;

	file << "!========================================================================= "<<endl;
	file << "!****************************  水  质  参  数  ****************************"<<endl;
	file << "!========================================================================= "<<endl;
	file << "!------------------------------AD组分相关参数------------------------" << endl;
	file << "!是否启动AD模块" << endl;
	file << "AD_options = F" << endl << endl;
	file << "!AD参数文件所在文件夹的名称" << endl;
	file << "AD_Name = WaterQuality1D" << endl << endl;
	file << "!AD水质种类" << endl;
	file << "AD_VariablesNum = 2" << endl<<endl;
	file << "!AD水质名称" << endl;
	file << "ADM_VariablesName = TSS, TN" << endl<<endl;
	file << "!------------------------------WQM组分相关参数------------------------" << endl;
	file << "!是否启动WQ模块" << endl;
	file << "WQM_options = F" << endl <<endl;
	file << "WQ参数文件所在文件夹的名称" << endl;
	file << "WQM_Name = WQM" << endl <<endl;
	file << "!WQ水质种类(固定)" << endl;
	file << "WQM_VariablseNum = 8" << endl <<endl;
	file << "!WQ水质名称（固定）" << endl;
	file << "WQM_VariablesName = DO, CBOD, PHY, NH4, NOX, ON, OPO4, OP" << endl<<endl;
	
	Log::GetLog()->WriteLogFile(INFO26);
}


void GrmsWriter::WriteManing(const vector<SINGLE> &sin)
{
	if (!sin.size()) return;

	string m_mann = GenerateFile(m_setting, "Manning.inp");

	FileOutputStream file(m_mann);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	file << "![uniform]" << endl;
	file << "![segments]" << endl<<endl;

	vector<SINGLE>::const_iterator iter;
	for (iter = sin.begin(); iter != sin.end(); iter++)
	{
		int size = (*iter).size;
		file << "[single]" << endl;
		file << setw(20) << (*iter).ID << setw(15) << size << endl;

		const vector<double> &rough = (*iter).rough;
		const vector<XY> &xy = (*iter).station;
		for (int i = 0; i < size; i++)
		{
			file << setw(15) << xy[i].x << setw(15) << xy[i].y;
			file << setw(10) << rough[i] << endl;
		}
		file << endl;
	}
	
	Log::GetLog()->WriteLogFile(INFO27);
}


void GrmsWriter::writePsource(const vector<PSOURCE> &sour)
{
	if (!sour.size()) return;

	string m_ps = GenerateFile(m_setting, "PSource.inp");

	FileOutputStream file(m_ps);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	vector<PSOURCE>::const_iterator iter;
	for (iter = sour.begin(); iter != sour.end(); iter++)
	{
		file << "[source]" << endl;
		file << setw(20) << (*iter).ID << setw(20) << (*iter).dm;
		file << setw(10) << (*iter).length << setw(10) << (*iter).ttype << endl;

		vector<XY>::const_iterator it;
		
		for (it = (*iter).serie.begin(); it != (*iter).serie.end(); it++)
			file << setw(15) << (*it).y;
		file << endl;
		for (it = (*iter).serie.begin(); it != (*iter).serie.end(); it++)
			file << setw(15) << (*it).x;
		file << endl;
		file << endl;
	}

	Log::GetLog()->WriteLogFile(INFO29);
}


void GrmsWriter::WriteBcSection(const vector<bcSECTION> &bcsec)
{
	if (!bcsec.size()) return;

	string m_bcsec = GenerateFile(m_bc, "bcCrossSection.inp");

	FileOutputStream file(m_bcsec);
	file.setf(ios::left);
	
	file << "!边界断面总数" << endl;
	file << bcsec.size() << endl;

	file << "! 边界序号 边界所在断面的序号  边界类型（1: 水位边界; 2: 流量边界;3:开边界） 边界名称" << endl;
	vector<bcSECTION>::const_iterator iter;
	for (iter = bcsec.begin(); iter != bcsec.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(15) << (*iter).dm << setw(10) << (*iter).type << endl;
	}

	Log::GetLog()->WriteLogFile(INFO30);
}


void GrmsWriter::WriteBcTime(const vector<bcTIME> &bctime)
{
	if (!bctime.size()) return;

	string m_bctime = GenerateFile(m_bc, "bcTimeSeries.inp");

	FileOutputStream file(m_bctime);
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(3);

	int serieN = bctime.size();
	int serieL = bctime[0].size;

	file << "!	时间序列长度 " << endl;
	file << serieN << endl;

	file << "!	时刻(小时)	BC1流量(m^3/s)"<<endl;
	for (int i = 0; i < serieL; i++)
	{
		file << setw(10) << i;

		vector<bcTIME>::const_iterator iter;
		for (iter = bctime.begin(); iter != bctime.end(); iter++)
		{
			file << setw(15) << (*iter).value[i].y;
		}
		file << endl;
	}

	Log::GetLog()->WriteLogFile(INFO31);
}


void GrmsWriter::WriteBcsluice(const vector<bcSLUICE>& bcsluice)
{
	if (!bcsluice.size()) return;

	string m_bcsluice = GenerateFile(m_structure, "bcSluices.inp");

	FileOutputStream file(m_bcsluice);
	file.setf(ios::left);

	file << "! 水闸边界断面总数" << endl;
	file << bcsluice.size() << endl;

	file << "! 水闸名称 断面 闸门宽 闸门个数 闸底高程 闸顶宽度 闸顶高程 最大开度 初始状态 流量系数" << endl;
	vector<bcSLUICE>::const_iterator iter;
	for (iter = bcsluice.begin(); iter != bcsluice.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(15) << (*iter).dm << endl;
	}

	Log::GetLog()->WriteLogFile(WARN15);
}


void GrmsWriter::WriteSluice(const vector<SLUICE>& sluice)
{
	if (!sluice.size()) return;

	string m_sluice = GenerateFile(m_structure, "Sluices.inp");

	FileOutputStream file(m_sluice);
	file.setf(ios::left);

	file << "[SluiceInnerNum]" << endl;
	file << sluice.size() << endl;

	file << "[SluiceInner]" << endl;
	file << "! 水闸编号 断面 类型 闸门宽度 最大开度 底程 闸孔个数 顶部高度 开启时间 顶部宽度 水闸名称" << endl;
	vector<SLUICE>::const_iterator iter;
	for (iter = sluice.begin(); iter != sluice.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(15) << (*iter).dm << endl;
	}

	file << "![Operation]" << endl;
	file << "!时间序列长度  !时刻（小时） 闸1 ..." << endl;


	Log::GetLog()->WriteLogFile(WARN16);
}


void GrmsWriter::WriteBcWeir(const vector<bcWEIR>& bcweir)
{
	if (!bcweir.size()) return;

	string m_bcweir = GenerateFile(m_structure, "bcWeirs.inp");

	FileOutputStream file(m_bcweir);
	file.setf(ios::left);

	file << "水闸边界断面总数" << endl;
	file << bcweir.size() << endl;

	file << "! 水闸名称 断面 堰宽 堰顶高程 流量系数" << endl;
	vector<bcWEIR>::const_iterator iter;
	for (iter = bcweir.begin(); iter != bcweir.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(15) << (*iter).dm << endl;
	}

	Log::GetLog()->WriteLogFile(WARN17);
}


void GrmsWriter::WriteWeir(const vector<grWEIR>& weir)
{
	if (!weir.size()) return;

	string m_weir = GenerateFile(m_structure, "Weirs.inp");

	FileOutputStream file(m_weir);
	file.setf(ios::left);

	file << "内部水堰个数" << endl;
	file << weir.size() << endl;

	file << "! 内部水堰编号 所在河段编号  堰宽 底程 " << endl;
	vector<grWEIR>::const_iterator iter;
	for (iter = weir.begin(); iter != weir.end(); iter++)
	{
		file << setw(15) << (*iter).ID << setw(15) << (*iter).dm << endl;
	}

	Log::GetLog()->WriteLogFile(WARN18);
}


void GrmsWriter::WritePump(const vector<grPUMP>& pump)
{
	if (!pump.size()) return;

	string m_pump = GenerateFile(m_structure, "Pumps.inp");

	FileOutputStream file(m_pump);
	file.setf(ios::left);

	vector<grPUMP>::const_iterator iter;
	for (iter = pump.begin(); iter != pump.end(); iter++)
	{
		file << "[pump]" << endl;
		file << setw(15) << (*iter).ID << setw(15) << (*iter).fromdm << setw(15) << (*iter).todm;
		file << "!水泵编号  !起始节点  !终止节点" << endl;
		file << setw(15) << "" << "!起泵水位  !停泵水位" << endl;
		file << setw(15) << "" "!时间序列" << endl;
		file << endl;
	}

	Log::GetLog()->WriteLogFile(WARN19);
}


void GrmsWriter::WritePond(const vector<POND>& pond)
{
	if (!pond.size()) return;

	string m_pond = GenerateFile(m_structure, "Ponds.inp");

	FileOutputStream file(m_pond);
	file.setf(ios::left);

	vector<POND>::const_iterator iter;
	for (iter = pond.begin(); iter != pond.end(); iter++)
	{
		file << "[pond]" << endl;
		file << "!蓄水池编号  !类型  !初始水深" << endl;
		file << setw(15) << (*iter).ID << setw(10) << (*iter).type << setw(15) << (*iter).cross;
		file << setw(15) << "" << "!曲线长度  !起泵水位  !停泵水位" << endl;
		file << setw(15) << "" << "!时间序列" << endl;
		file << endl;
	}

	Log::GetLog()->WriteLogFile(WARN20);
}


