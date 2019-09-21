#pragma once
#include<vector>
#include<string>

using std::vector;
using std::string;

namespace grms2swmm
{
	//***************************************************************************
	//*** universal exceptions **************************************************
	const string SS_ERR1 = "\r\nGRMS2SWMM MODUL: (%s) file or folder doesn't exist.\r\n";
	const string SS_ERR2 = "\r\nGRMS2SWMM MODUL: vector data doesn't exist.\r\n";
	const string SS_ERR3 = "\r\nSWMM2GRMS MODUL: transect resample failed at (%s).\r\n";
	const string SS_ERR4 = "\r\nSWMM2GRMS MODUL: transect create failed at (%s).\r\n";
	 


	//***************************************************************************
	//*** universal variable types **********************************************
	static const double __DELTA = 1e-5;

	enum STATUS { YES, NO };

	template<typename T>
	struct	XY { T  x, y; };

	template<typename T>
	struct SET
	{
		string ID;
		int setNum;
		vector<T> sets;
	};



	//***************************************************************************
	//*** variable types for GrmsReader *****************************************
	struct DM
	{
		string ID;         // name of dm.
		STATUS mark = NO;  // NO for roughness unassigned, YES for assigned.
	};                 
	
	typedef XY<double>   STATION;          // stations of transect.

	typedef SET<STATION> TRANSECT;         // transects of reach.

	typedef SET<string>  REACH;            // reaches of river.

	typedef SET<XY<double>> COORDINATE;    // coordinate of transect.

	typedef SET<string>  JUNCTION;         // junctions of river.

	typedef struct
	{
		string dm1 = "";
		string dm2 = "";
		double rough;
	}SEGMENT;        // [segment] in mannings.

	typedef SET<STATION>  SINGLE;          // [single] in mannings.
	
	typedef struct
	{
		double uniform;
		vector<SEGMENT> segment;
		vector<SINGLE>  single;
	}MANNING;        // mannings of transect, have no ID.

	typedef SET<XY<double>> BC;            // bc series.
	


	//***************************************************************************
	//*** variable types for GrmsToNode *****************************************
	 enum OutfallType{ FREE, NORMAL, FIXED, TIMESERIES };
	 
	typedef struct
	 {
		 string ID;
		 string data = "";
		 vector<XY<double>> series;
	 }swmmTimeseries;// data of swmm_timeseries.    

	typedef struct
	{
		string ID;
		double elevation;
		double maxDepth, initDepth, surDepth;
		double aponded;
	}swmmJUNCTION;   // data of swmm_junction.

	typedef struct
	{
		string ID;
		double elevation;
		string type = "FREE";
		string stage = "";
		string gated = "NO";
		string routeTo = "";
	}swmmOUTFALL;    // data of swmm_outfall.
	
	typedef struct
	{
		string ID;
		string constituent = "FLOW";
		string series = "";
		string type = "FLOW";
		double mFactor = 1.0;
		double sFactor = 1.0;
		double baseLine = 0;
		string pattern = "";
	}swmmINFLOW;     // data of swmm_inflow.

	typedef struct
	{
		string ID;
		XY<double> xy;
	}swmmCOORDINATE; // data of swmm_coordinate.

	

	//***************************************************************************
	//*** variable types for GrmsToLink *****************************************
	typedef struct
	{
		string ID;
		string upNode, downNode;
		double length;
		double roughness = 0.01;
		double inOff = 0, outOff = 0;
		double initFlow = 0, maxFlow = 0;
	}swmmCONDUIT;    // data of swmm_conduit.
	
	typedef struct
	{
		string ID;
		string shape = "IRREGULAR";	
		string geom1;                 
		double geom2 = 0, geom3 = 0, geom4 = 0;
		int barrel = 1, culvert = 0;
	}swmmSECTION;    // data of swmm_section.


	
	//***************************************************************************
    //*** variable types for GrmsToTransect *************************************
	struct NC                 // NC line of a transect.
	{
		double Nleft = 0.01;
		double Nright = 0.01;
		double Nchanl = 0.01;
	};

	struct X1                 // X1 line of a transect.
	{
		string ID;
		int Nstation;
		double Xleft = 0;
		double Xright = 0;
		double Lfactor = 0;
		double Wfactor = 0;
		double Eoffset = 0;
	};

	typedef struct 
	{
		string ID;
		NC rough;
		X1 param;
		vector<STATION> stations;
	}swmmTRANSECT;   // data of swmm_transect.

	

	//***************************************************************************
}
