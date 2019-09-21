/*-----------------------------------------------------------
vars.h -- SwmmToGrms程序中全局常量、宏函数、变量和数据结构。

-------------------------------------------------------------*/



/*--- 头文件 ------------------------------------------------*/
#pragma once
#include<vector>
#include<string>



/*--- 命名空间 ----------------------------------------------*/
using std::vector;
using std::string;



/*--- SwmmToGrms命名空间常量、变量、数据结构 ----------------*/
namespace SwmmToGrms
{
	/*--- 通用数据类型 --------------------------------------*/
	typedef struct
	{
		double x;
		double y;
	}XY;

	typedef struct
	{
		string ID = "";
		vector<double> num;
		vector<string> str;
	}DATA;



	/*--- swmm数据类型 --------------------------------------*/
	enum SwmmDataType { Node,Pipe,Time,Transect };
	enum NodeType { JUNCTION, OUTFALL, DIVIDER, STORAGE  };
	enum PipeType { CONDUIT, PUMP, ORIFICE, WEIR, OUTLET };
	enum TranType { RECT_OPEN, TRAPEZOIDAL, TRIANGULAR, IRREGULAR,OTHER };
	enum PumpCurveType {PUMP1,PUMP2,PUMP3};



	typedef struct
	{
		string     ID = "";
		NodeType   type;
		string     inflow="";
		double     z;
		double     initdepth;
		double     x;
		double     y;
	}NODE;

	typedef struct
	{
		string     ID = "";
		bool       useflag=FALSE;
		PipeType   ptype;
		TranType   ttype;
		string     fromnode;
		string     tonode;
		double     initflow=0;
		string     shape;
	}PIPE;

	typedef struct
	{
		string      ID = "";
		vector<XY>  value;
	}TIME;
	
	typedef struct
	{
		string      ID = "";
		double      leftrough;
		double      rightrough;
		double      channelrough;
		int         nstation;
		double      leftstation;
		double      rightstation;
		double      wfactor;
		double      eoffset;
		vector<XY>  stations;
	}TRANSECT;
	
	typedef struct
	{
		string      startdate;
		string      enddate;
		string      starttime;
		string      endtime;
		string      routstep;
		string      rptstep;
		int         thread;
		int         maxtrial;
		double      headtol;
	}OPTION;

	typedef struct
	{
		string        ID = "";
		PumpCurveType type;
		vector<XY>    value;
	}PumpCurve;

	typedef struct
	{
		string  ID="";
		vector<XY>  value;
	}StorageCurve;



	/*--- convertor数据类型 ---------------------------------*/
	enum NodeSite { Start, End, Inner, Cross, Out };

	typedef struct
	{
		string     ID = "";
		int        count;
		int        num;
		bool       useflag = FALSE;
		NodeSite   site;
	}NODESITE;

	typedef struct
	{
		string          ID = "";
		vector<string>  pipes;
		vector<string>  nodes;
	}TEMPHD;



	/*--- grms数据类型 --------------------------------------*/
	enum PUMPTYPE {ON, OFF};
	enum LINKTYPE {OPEN, CLOSED};

	typedef struct
	{
		string      ID = "";
		int         size;
		int         tran;
		double      length;
		vector<XY>  stations;
	}DM;

	typedef struct
	{
		string      ID = "";
		int         size;
		vector<DM>  dm;
	}HD;

	typedef struct
	{
		string      ID = "";
		double      x;
		double      y;
	}LOCATION;

	typedef struct
	{
		string ID = "";
		double dis;
	}DST;

	typedef struct
	{
		string          ID = "";
		int             size;
		vector<string>  dmid;
	}grJUNCTION;	    
					    
	typedef struct	    
	{				    
		string          ID = "";
		int             size=0;
		vector<double>  rough;
		vector<XY>      station;
	}SINGLE;		    

	typedef struct
	{
		string     ID = "";
		string     dm;
		string     ttype="hour";
		int        length;
		vector<XY> serie;
	}PSOURCE;

	//typedef struct
	//{
	//	string   startdm;
	//	string   enddm;
	//	double   rough;
	//}SEGMENT;

	//typedef struct
	//{
	//	vector<SEGMENT> segment;
	//	vector<SINGLE>  single;
	//}MANING;

	typedef struct
	{
		string         ID = "";
		int            size;
		vector<XY>     value;
	}bcTIME;

	typedef struct
	{
		int      method = 3;
		int      ic_options = 0;
		double   gama = 0.75;
		double   dz_ic;
		double   q_ic;
		double   dt;
		double   t0;
		double   dt_out = 60.0;
		double   tfinal;
		double   dt_report;
		double   dt_monitor = 1800.0;
		int      threadsnum;
		int      maxinter;
		double   epsz;
		double   epsq=0.001;
		double   g = 9.79;
		double   fr1 = 0.99;
		double   fr2 = 1.1;
	}SET;

	typedef struct
	{
		int      ID;
		string   dm;
		int      type=3;
	}bcSECTION;

	typedef struct
	{
		string   ID = "";
		string   dm;
	}bcSLUICE;

	typedef struct
	{
		string   ID = "";
		string   dm;
	}SLUICE;	

	typedef struct
	{
		string   ID = "";
		string   dm;
	}bcWEIR;

	typedef struct
	{
		string   ID = "";
		string   dm;
	}grWEIR;

	typedef struct
	{
		string   ID = "";
		string   fromdm;
		string   todm;
	}grPUMP;

	typedef struct
	{
		string ID = "";
		string cross;
		int    type=1;
	}POND;


}



