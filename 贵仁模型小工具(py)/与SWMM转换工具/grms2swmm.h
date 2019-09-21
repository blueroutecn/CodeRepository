#pragma once

//*** head file *****************************************************************
#include<iomanip>
#include<algorithm>
#include<math.h>
#include<sstream>
#include<numeric>

#include<Poco/FileStream.h>
#include<Poco/File.h>
#include<Poco/Path.h>
#include<Poco/Exception.h>
#include<Poco/Format.h>

#include"DataStreamReader.h"
#include"variable.h"

//*** namespace *****************************************************************
using std::ios;
using std::endl;
using std::setw;

using Poco::DataException;
using Poco::FileExistsException;
using Poco::FileOutputStream;
using Poco::FileInputStream;
using Poco::format;
using Poco::Path;
using Poco::File;


namespace grms2swmm
{	

	//*** class GrmsReader ******************************************************
	/*
	- FUNCTION:
	  1. to get data from grms files.

	- METHOD:
	  1.ReadGrmsData(), to read data from grms files, must be called before Return...().

	- NOTICE:
	  1.constructor GrmsReader() will search "grms" in current direction to assign _grmsPath.
	*/
	class GrmsReader
	{
	public:
		GrmsReader();
		GrmsReader(const string& grms);
		~GrmsReader(){}
		
		static void CheckFileExists(const string& file);
		template<typename T>
		inline size_t CheckDataExists(const vector<T>& data)const            
		{
			if (data.size() < 1)  throw DataException(SS_ERR2);
			return data.size();
		}
		template<typename T>
		inline size_t FindDataIndex(string id, const vector<T> &data)const
		{
			if (data.size() < 1)  throw DataException(SS_ERR2);

			size_t num = find_if(begin(data), end(data), [&id](T t) {return t.ID == id; }) - data.begin();

			if (num != data.size()) return num;
			else throw DataException(format(SS_ERR2, id));
		}
				
		void ReadGrmsData();
		const vector<REACH>&      ReturnReach()const;
		const vector<TRANSECT>&   ReturnTransect()const;
		const vector<COORDINATE>& ReurnCoordinate()const;
		const vector<SINGLE>&     ReturnRoughness()const;
		const vector<JUNCTION>&   ReturnJunction()const;
		const vector<JUNCTION>&   ReturnNewCross()const;
		const vector<BC>&         ReturnBC()const;
		
	private:		
		void ReadHdFile();
		void ReadLocationFile();
		void ReadJunctionFile();
		void ReadManningFile();
		void ReadBcFiles();

		// to convert roughness to the format of swmm.
		void ConvertManning(const MANNING &_manning);

		// to extract transects' name from _reaches, used in ConvertManning().
		void GenerateDM(vector<DM> &transects);

		// to convert [single] part of _manning to new format, used in ConvertManning().
		void ConvertSingle(vector<DM> &transects);

		// to convert [segment] part of _manning to new format, used in ConvertManning().
		void ConvertSegment(vector<DM> &transects);

		// to convert [uniform] part of _manning to new format, used in ConvertManning().
		void ConvertUniform(vector<DM> &transects);

		// to generate new crosses for swmm, through old junctions of grms.
		void GenerateNewCross(const vector<JUNCTION>& junctions);

		// to turn unsure-type path to absolute one, used in constructor.
		const string AbsolutePath(const string &grms);

		// to turn integer to string, as an inline function, used in ReadHdFile() &  GenerateNewCross().
		string num2str(double num)
		{
			std::stringstream ss;
			ss << num;
			return ss.str();
		}
				
	private:
		string             _grmsPath;    // to storage path of folder containing grms files .
		MANNING            _manning;     // to storage orignal roughness of transects.
		vector<REACH>      _reaches;     // to storage reaches' data, including which conduits were connected as one reach.
		vector<TRANSECT>   _transects;   // to storage transects' shape data.
		vector<COORDINATE> _coordinates; // to storage transects' coordinate.
		vector<SINGLE>     _roughness;   // to storage roughness formated in swmm format.
		vector<JUNCTION>   _junctions;   // to storage junctions' data, including correlative transects. 
		vector<JUNCTION>   _newCrosses;  // to storage data of old junctions with newly create cross node.
		vector<BC>         _bcSeries;    // to storage data of time series of bcCross.
	};
	
		
	
	//*** class GrmsToNode **************************************************
    /*
	- FUNCTION:
	  1. to generate [JUNCTIONS], [OUTFALLS], [INFLOWS], [COORDIANTES],[TIMESERIES] parts for swmm file.

	- METHOD:
	  1. GenerateSwmmNode(), to generate swmm data, must be called before Return...().

	- NOTICE:
	  1. none.
    */
	class GrmsToNode
	{							
	public:						
		explicit GrmsToNode(const GrmsReader& reader);
		~GrmsToNode(){}
			
		void GenerateSwmmNode();
		const vector<swmmTimeseries>& ReturnSwmmTimeseries()const;
		const vector<swmmJUNCTION>&   ReturnSwmmJunction()const;
		const vector<swmmOUTFALL>&    ReturnSwmmOutfall()const;
		const vector<swmmINFLOW>&     ReturnSwmmInflow()const;
		const vector<swmmCOORDINATE>& ReturnSwmmCoordinate()const;
		
	private:
		void GenerateTimeseries(const vector<BC>& bc);
		void GenerateJunction(const vector<TRANSECT>& transects);
		void GenerateCoordinate(const vector<COORDINATE>& coordinates);
		void GenerateOutfall(const vector<JUNCTION>& junctions, const vector<REACH>& reaches, const vector<BC>& bc);
		void GenerateInflow(const vector<REACH>& reaches, const vector<BC>& bc);
			 
		// to remove duplicate junction nodes ind _swmmJunctons and _swmmcoordinates.
		void RemoveDuplicateJunction();

		// to create (x,y,z) for cross node, _swmmJunctions and _swmmCoordinates both changed.
		void CreateCrossCoor();

		// to create (x,y) for cross node, used in CreateCrossCoor() after GenerateCoordinate().
		XY<double> CreateXY(const JUNCTION& jun, XY<double>& xy);

		// to create z-coor for cross node, used in CreateCrossCoor() after GenerateJunction().
		double CreateZ(const JUNCTION& jun);

		string num2str(size_t num)
		{
			std::stringstream ss;
			ss << num;
			return ss.str();
		}

	private:
		const GrmsReader&      _reader;    
		vector<swmmTimeseries> _swmmTimeseries;    // to storage data of swmm [TIMESERIES].
		vector<swmmJUNCTION>   _swmmJunctions;     // to storage data of swmm [JUNCTIONS].
		vector<swmmOUTFALL>    _swmmOutfalls;      // to storage data of swmm [OUTFALLS].
		vector<swmmINFLOW>     _swmmInflows;       // to storage data of swmm [INFLOWS].
		vector<swmmCOORDINATE> _swmmCoordinates;   // to storage data of swmm [COORDIANTES].
	};



	//*** class GrmsToLink ******************************************************
	/*
	- FUNCTION:
	  1. to generate [CONDUITS], [XSECTIONS] parts of swmm file.

	- METHOD:
	  1. GenerateSwmmLink(), to generate data for swmm file, must be called before Return...().

	- NOTICE:
	  1. none.
	*/
	class GrmsToLink
	{
	public:
		explicit GrmsToLink(const GrmsReader& reader);	
		~GrmsToLink(){}
		
		void GenerateSwmmLink();
		const vector<swmmCONDUIT>& ReturnSwmmConduit()const;
		const vector<swmmSECTION>& ReturnSwmmSection()const;

	private:
		void GenerateConduit();
		void ResetJunction();

		// to turn integer to string, used in GenerateConduit().
		string num2str(double num)
		{
			std::stringstream ss;
			ss << num;
			return ss.str();
		}

		// to compute conduit's length through (x,y),used in GenerateConduit().
		double length(size_t upNode, size_t downNode)
		{
			const vector<COORDINATE>& coordinate = _reader.ReurnCoordinate();

			double diffX = coordinate[upNode].sets[0].x - coordinate[downNode].sets[0].x;
			double diffY = coordinate[upNode].sets[0].y - coordinate[downNode].sets[0].y;

			return sqrt(diffX*diffX + diffY * diffY);
		}

	private:
		const GrmsReader&   _reader;
		vector<swmmCONDUIT> _swmmConduits; // to storage data of swmm [CONDUITS].
		vector<swmmSECTION> _swmmSections; // to storage data of swmm [SECTIONS].
	};

	

	//*** class GrmsToTransect **************************************************
	/* 
	- FUNCTION:
	  1.  to generate [TRANSECTS] part of swmm file.

	- MEHTOD:
	  1. MaxStations(), get and return num of stations used for all new transects here.
	  2. GenerateSwmmTransect(), to generate new transects, must be called before Return...().

	- NOTICE:
	  1. none.
	*/
	class GrmsToTransect
	{
	public:
		explicit GrmsToTransect(const GrmsReader& reader);
		~GrmsToTransect(){}
		
		int MaxStation();
		void GenerateSwmmTransect();
		const vector<swmmTRANSECT>& ReturnSwmmTransect()const;

	private:
		void ResampleTransect();

		// to judge the new bank-station exists, if not, then create a new station for it, used in  GenerateSwmmStation().
		void GenerateBankStation(const swmmTRANSECT &transect, vector<STATION> &newXY);
		
		// to generate NC and X1 line for swmm transect, used in GenerateSwmmTransect().
		void GenerateNCandX1(size_t rough1, size_t rough2, swmmTRANSECT &transect);

		// to generate stations for swmm transect, used in GenerateSwmmTransect().
		void GenerateSwmmStation(size_t tran1, size_t tran2, swmmTRANSECT &transect);

		// to connect x-coord with length, used in ResampleTransect().
		void ConnectXtoL(const TRANSECT &tran, double &width, vector<XY<double>> &xl);
		
		// to create new (x,y) through l(l -> x -> y), ResampleTransect().
		void CreateStation(double l, STATION &station,const vector<STATION> &xl,const TRANSECT &tran);

		// to turn integer to string, used in GenerateSwmmTransect().
		string num2str(size_t num)
		{
			std::stringstream ss;
			ss << num;
			return ss.str();
		}
		
	private:
		const GrmsReader&    _reader;
		const int            _station;         // To storage the stations-num used in all new transects.
		vector<TRANSECT>     _newTransects;    // to storage resample-transects' shape data.
		vector<swmmTRANSECT> _swmmTransects;   // to storage data of swmm [TRANSECTS].
	};



	//*** class GrmsToSwmm ******************************************************
     /*
	 - FUNCTION:
	   1. to generate swmm file.

	 - METHOD:
	   1. WriteSwmmFile(), to write swmm file.

	 - NOTICE:
	   1. if no grms path, GrmsToSwmm() will search "swmm.inp" in current direction to assign _swmmPath.

	 */
	class GrmsToSwmm
	{
	public:
		GrmsToSwmm(const string &path, const GrmsToNode &node, const GrmsToLink &link, const GrmsToTransect &tran);
		GrmsToSwmm(const GrmsToNode &node, const GrmsToLink &link, const GrmsToTransect &tran);
		~GrmsToSwmm(){}

		void WriteSwmmFile();

	private:
		string num2str(double num)
		{
			std::stringstream ss;
			ss << num;
			return ss.str();
		}


		void OutTitle(FileOutputStream &file);
		void OutOptions(FileOutputStream &file);

		void OutEvaporation(FileOutputStream &file);

		// no dividers and storages.
		void OutJunctions(FileOutputStream &file);
		void OutOutfalls(FileOutputStream &file);

		// no pumps ,weirs, orifices and outlets.
		void OutConduits(FileOutputStream &file);

		void OutXsections(FileOutputStream &file);
		void OutTransects(FileOutputStream &file);

		void OutInflows(FileOutputStream &file);
		void OutTimeseries(FileOutputStream &file);

		void OutReport(FileOutputStream &file);
		void OutTags(FileOutputStream &file);
		void OutMap(FileOutputStream &file);

		void OutCoordinates(FileOutputStream &file);

		void OutVertices(FileOutputStream &file);
		void OutProfiles(FileOutputStream &file);

	private:
		string _swmmPath;
		const GrmsToNode&      _node;
		const GrmsToLink&      _link;
		const GrmsToTransect&  _transect;
	};

	

	//***************************************************************************
	//注意:
	//(1) grms 文件必须是自动生成的，没有改动.                          ---06/06.
	//(2) 现在仅支持基本的拓扑结构重建，更多具体参数设置暂不支持.       ---06/06.
	//***************************************************************************
}







using namespace grms2swmm;

int main()
{
	string str = "F:\\tempfile\\grms";
	string str2 = "F:\\tempfile\\test.inp";

	try
	{
		GrmsReader test(str); test.ReadGrmsData();

		GrmsToNode test2(test); test2.GenerateSwmmNode();

		GrmsToTransect test4(test); test4.GenerateSwmmTransect();

		GrmsToLink test3(test); test3.GenerateSwmmLink();

		GrmsToSwmm test5(str2, test2, test3, test4);
		test5.WriteSwmmFile();
	}
	catch (DataException &exc)
	{
		std::cout << exc.displayText() << endl;
    }

	return 0;
}














