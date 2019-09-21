/*-----------------------------------------------------------
SwmmToGrms.h -- SwmmToGrms程序中类和函数的原型。

-------------------------------------------------------------*/

/*--- 调试日志 ----------------------------------------------*/
#define __DEBUG



/*--- 头文件 ------------------------------------------------*/
#pragma once
#include<memory>
#include<Poco/FileStream.h>
#include<Poco/File.h>
#include<Poco/Path.h>
#include<Poco/Exception.h>
#include<Poco/Format.h>

#include"DataStreamReader.h"
#include"vars.h"


/*--- 命名空间 ----------------------------------------------*/
using std ::unique_ptr;
using std ::string;
using std ::vector;
using Poco::DataException;
using Poco::FileExistsException;
using Poco::FileOutputStream;
using Poco::FileInputStream;
using Poco::Path;
using Poco::File;
using Poco::format;



/*--- SwmmToGrms命名空间类和函数 ----------------------------*/
namespace SwmmToGrms
{
	/*--- FindIndex函数 -------------------------------------*/
	template<typename T>
	size_t FindIndex(const string id, const vector<T> &vec)
	{
		typename vector<T>::const_iterator iter;

		for (iter = vec.begin(); iter != vec.end(); iter++)
		{
			if ((*iter).ID == id) break;
		}

		if (iter != vec.end())
		{
			return std::distance(vec.begin(), iter);
		}
		else
		{
			Log::GetLog()->WriteLogFile(format(ERR2, id));
			return -1;
		}
	}
	 

	
	/*--- log文件类 -----------------------------------------*/
	class Log
	{
	public:
		static Log * GetLog(string logfile="");
		string ReturnLogFile()const;

		void   WriteLogFile(const string &text);
		template<typename T>
		void   WriteLogFile(const string &text, const vector<T> &vec)
		{
			FileOutputStream logstream(m_logfile, ios::app);
			m_count++;

			logstream << "\r\n ";
			logstream << setw(5) << m_count << "> " << SystemTime() << ", ";
			logstream << text << "\r\n"<<setw(20)<<"";

			typename vector<T>::const_iterator iter;
			int num = 1;
			for (iter = vec.begin(); iter != vec.end(); iter++, num++)
			{
				logstream << setw(20) << setiosflags(ios::left) << (*iter).ID;
				if (num % 5 == 0) logstream << "\r\n"<<setw(20)<<"";
			}

			if ((num-1) % 5 != 0) logstream << "\r\n";
		}
		
	private:
		static int              m_count;
		static string           m_logfile;
		static unique_ptr<Log>  m_log;   

	private:
		Log();
		Log(const Log&) {}
		Log(const string &logfile);
		Log& operator = (const Log&) {};
	};



	/*--- swmm文件类 ----------------------------------------*/
	class SwmmInp
	{
	public:
		static SwmmInp* GetSwmmInp(string inpfile = "");
		string ReturnInpFile()const;

	private:
		static string              m_inpfile;
		static unique_ptr<SwmmInp> m_swmminp;

	private:
		SwmmInp();
		SwmmInp(const SwmmInp&) {}
		SwmmInp(const string &inpfile);
		SwmmInp& operator = (const SwmmInp&) {}
	};



	/*--- swmmnode类 ----------------------------------------*/
	class SwmmNode
	{
	public:
		SwmmNode(SwmmInp* swmm);
		const vector<NODE>& ReturnSwmmNode()const;

	private:
		SwmmNode() {}
		SwmmNode(const SwmmNode&) {}
		SwmmNode& operator = (const SwmmNode&) {}

		void ReadAllNode();
		void ReadInflownode();
		void ReadCoordinate();

	private:
		string        m_inpfile;
		vector<NODE>  m_node;
		vector<NODE>  m_specnode;
	};



	/*--- swmmpipe类 ----------------------------------------*/
	class SwmmPipe
	{
	public:
		SwmmPipe(SwmmInp* swmm);
		const vector<PIPE>& ReturnSwmmPipe()const;

	private:
		SwmmPipe() {}
		SwmmPipe(const SwmmPipe&) {}
		SwmmPipe& operator = (const SwmmPipe&) {}

		void ReadAllPipe();
		void ReadXSection();
		
	private:
		string       m_inpfile;
		vector<PIPE> m_pipe;
		vector<PIPE> m_specpipe;
		vector<PIPE> m_specsect;
	};



	/*--- swmmtime类 ----------------------------------------*/
	class SwmmTime
	{
	public:
		SwmmTime(SwmmInp* swmm);
		const vector<TIME>& ReturnSwmmTime()const;

	private:
		SwmmTime() {}
		SwmmTime(const SwmmTime&){}
		SwmmTime& operator = (const SwmmTime&) {}

		void ReadSwmmTime();
		
	private:
		string       m_inpfile;
		vector<TIME> m_time;
	};



	/*--- swmmtransect类 ------------------------------------*/
	class SwmmTransect
	{
	public:
		SwmmTransect(SwmmInp* swmm);
		const vector<TRANSECT>& ReaturnSwmmTransect()const;

	private:
		SwmmTransect() {}
		SwmmTransect(const SwmmTransect&){}
		SwmmTransect& operator=(const SwmmTransect &) {}

		void ReadSwmmNodePipe();
		void ReadSwmmTransect();
		void GetUnirregularTransect();
		
		TRANSECT RectopenTransect(DataStreamReader &read, string conduit);
		TRANSECT TrapezoidalTransect(DataStreamReader &read,string conduit);
		TRANSECT TriangularTransect(DataStreamReader &read,string conduit);
		
	private:
		string           m_inpfile;
		vector<DATA>     m_node;
		vector<DATA>     m_pipe;
		vector<TRANSECT> m_transect;
		vector<TRANSECT> m_spectran;
	};



	/*--- swmmoption类 --------------------------------------*/
	class SwmmOption
	{
	public:
		SwmmOption(SwmmInp* swmm);
		const OPTION& ReaturnSwmmOption()const;

	private:
		SwmmOption() {}
		SwmmOption(const SwmmOption&){}
		SwmmOption& operator=(const SwmmOption&) {}

		void ReadSwmmOption();

	private:
		string m_inpfile;
		OPTION m_option;
	};
	


	/*--- setnodesite函数 -----------------------------------*/
	const vector<NODESITE> SetNodeSite(const vector<NODE>& node, const vector<PIPE>& pipe);
	


	/*--- junctionconvertor类 -------------------------------*/
	class JunctionConvertor
	{
	public:
		JunctionConvertor(const vector<NODE>& node,const vector<NODESITE>& site,
			const vector<TIME>& time);

		const vector<grJUNCTION> &ReturnGrJunction()const;
		const vector<LOCATION>   &ReturnLocation()const;
		const vector<bcSECTION>  &ReturnBcSection()const;
		const vector<PSOURCE>    &ReturnPsource()const;

	private:
		void GenerateJunction();
		void GenerateLocation();
		void GenerateBcSection();
		void GeneratePsource();

	private:
		vector<grJUNCTION> m_junc;
		vector<LOCATION>   m_loca;
		vector<bcSECTION>  m_bcsect;
		vector<SINGLE>     m_maning;
		vector<PSOURCE>    m_psource;

	private:
		const vector<TIME>      &m_time;
		const vector<NODE>      &m_node;
		const vector<NODESITE>  &m_nodesite;
	};



	/*--- hdconvertor类 -------------------------------------*/
	class HdConvertor
	{
	public:
		HdConvertor(const vector<TRANSECT> &tran,const vector<PIPE> &pipe,
			const vector<NODE> &node,const vector<NODESITE> &site);

		const vector<HD>&       ReturnHd()const;
		const vector<SINGLE>&   ReturnManing()const;

	private:
		void MatchSpecPipeDm();
		void GenerateDm();
		void GenerateTempHd();
		string SearchEachReach(TEMPHD &temp, string id);
		void GenerateHd();

		void GenerateSingle(SINGLE &single,size_t dm);
		void GenerateManing();

	private:
		vector<DM>      m_dm;
		vector<TEMPHD>  m_temphd;
		vector<HD>      m_hd;
		vector<SINGLE>  m_single;

	private:
		vector<NODESITE>         m_nodesite;
		vector<PIPE>             m_pipe;
		const vector<TRANSECT>  &m_tran;
		const vector<NODE>      &m_node;
	};


	/*--- timeconvertor类 -----------------------------------*/
	class TimeConvertor
	{
	public:
		TimeConvertor(const vector<TIME> &time);

		const vector<bcTIME>& ReturnGcTime()const;

	private:
		void GenerateTime();
		int CheckTime();

	private:
		const vector<TIME>&  m_time;
		vector<bcTIME>       m_bctime;
	};


	/*--- dstconvertor类 ------------------------------------*/
	class DstConvertor
	{
	public:
		DstConvertor(const vector<LOCATION> &loc, const vector<HD> &hd);

		const vector<DST>& ReturnDst()const;

	private:
		void GenerateDst();

	private:
		vector<DST>             m_dst;
		const vector<LOCATION>& m_loc;
		const vector<HD>&       m_hd;
	};

	
	/*--- setconvertor类 ------------------------------------*/
	class SetConvertor
	{
	public:
		SetConvertor(const OPTION &op, const vector<NODE> &n, const vector<PIPE> &p);

		const SET& ReturnSet()const;

	private:
		void GenerateSet();
		double GenerateDzic();
		double GenerateQic();
		vector<double> GenerateTime();

		vector<int> SeparateTimeStr(const string str, const string delim=":");

	private:
		SET                  m_set;
		const OPTION&        m_option;
		const vector<NODE>&  m_node;
		const vector<PIPE>&  m_pipe;
	};


	/*--- sluiceconvertor类 ------------------------------------*/
	class SluiceConvertor
	{
	public:
		SluiceConvertor(const vector<PIPE> &pipe, const vector<NODESITE> &site);

		const vector<bcSLUICE>& ReturnBcSluice()const;
		const vector<SLUICE>&   ReturnSluice()const;

	private:
		void GenerateSluice();

	private:
		vector<bcSLUICE>        m_bcsluice;
		vector<SLUICE>          m_sluice;
		const vector<PIPE>&     m_pipe;
		const vector<NODESITE>& m_node;
	};

	
	/*--- weirconvertor类 -----------------------------------*/
	class WeirConvertor
	{
	public:
		WeirConvertor(const vector<PIPE> &pipe, const vector<NODESITE> &site);

		const vector<bcWEIR>& ReturnBcWeir()const;
		const vector<grWEIR>& ReturnWeir()const;

	private:
		void GenerateWeir();

	private:
		vector<bcWEIR>          m_bcweir;
		vector<grWEIR>          m_weir;
		const vector<PIPE>&     m_pipe;
		const vector<NODESITE>& m_node;
	};
	

	/*--- pumpconvertor类 -----------------------------------*/
	class PumpConvertor
	{
	public:
		PumpConvertor(const vector<PIPE> &p,const vector<NODESITE> &n);

		const vector<grPUMP>& ReturnGrPump()const;

	private:
		void GenerateGrPump();

	private:
		vector<grPUMP>            m_pump;
		const vector<PIPE>&       m_pipe;
		const vector<NODESITE>&   m_node;
	};
	

	/*--- pondconvertor类 -----------------------------------*/
	class PondConvertor
	{
	public:
		PondConvertor(const vector<NODE> &node, const vector<NODESITE> &site,
			const vector<grJUNCTION> &cross);

		const vector<POND>& ReturnPond()const;

	private:
		void GeneratePond();
		string FindGrJunction(string pond);

	private:
		vector<POND>              m_pond;
		const vector<NODE>&       m_node;
		const vector<NODESITE>&   m_site;
		const vector<grJUNCTION>& m_cross; 
	};



	/*--- grmswriter类 --------------------------------------*/
	class GrmsWriter
	{
	public:
		GrmsWriter();
		GrmsWriter(const string grms);

		void WriteHd(const vector<HD>& hd);
		void WriteDst(const vector<DST>& dst);
		void WriteJunction(const vector<grJUNCTION>& junc);
		void WriteLocation(const vector<LOCATION>& loc);

		void WriteManing(const vector<SINGLE>& sin);
		void WriteSeting(const SET& set);
		void writePsource(const vector<PSOURCE>& sour);

		void WriteBcSection(const vector<bcSECTION>& bcsection);
		void WriteBcTime(const vector<bcTIME>& bctime);

		void WriteBcsluice(const vector<bcSLUICE>& bcsluice);
		void WriteSluice(const vector<SLUICE>& sluice);
		void WriteBcWeir(const vector<bcWEIR>& bcweir);
		void WriteWeir(const vector<grWEIR>& weir);
		void WritePump(const vector<grPUMP>& pump);
		void WritePond(const vector<POND>& pond);

	private:
		void   GenerateFolder(const string &path);
		string GenerateFile(const string &folder, const string &file);

	private:
		string m_structure;
		string m_setting;
		string m_topo;
		string m_bc;
	};


}



