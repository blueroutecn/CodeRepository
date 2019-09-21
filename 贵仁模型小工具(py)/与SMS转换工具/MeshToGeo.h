#pragma once


//***mesh转geo, 2018.04.24*****************************************

#include<iomanip>
#include <io.h>
#include<vector>

#include<Poco/FileStream.h>
#include<Poco/File.h>
#include<Poco/Path.h>
#include<Poco/Exception.h>
#include<Poco/Format.h>

#include"DataStreamReader.h"

using std::string;
using std::cout;
using std::endl;
using std::ios;
using std::setprecision;
using std::setw;
using std::vector;
//using std::istream;
using namespace Poco;

#define MESHFILE_ERR "File(%s): not exist."
#define DATA_ERR "Num of nodes in nodefile(%d) and bedfile(%d) not match."



#if defined( _WIN32)
#define LINEFEED "\r\n"
#elif defined( __linux__)||defined(__linux)
#define LINEFEED "\n"
#elif defined(__unix__)||defined(__unix)
#define LINEFEED "\n"
#elif defined(__APPLE__)&&defined(TARGET_OS_MAC)
#define LINEFEED "\r"
#else 
throw Exception("Unkown system.");
#endif


template<typename T>
struct MESHDATA
{
	T node1;
	T node2;
	T node3;
};

struct XY
{
	double x;
	double y;
};



//不能修改自动生成的mesh文件，从而保证各文件间数据对应。
 class  MeshToGeo 
{
public:

	MeshToGeo(const string &filepath);
	~MeshToGeo();
	
	void GetEleData();
	void GetNodeData();
	void CreatGeofile();

	int NumOfEles();
	int NumOfNodes();

private:
	string path;
	vector<MESHDATA<int>> eles;
	vector<double>nodes_z;
	vector<XY>nodes_xy;
};




#include <io.h>
int main(int argc, char* argv[])
{
	try
	{
		string str;
		Path path;

		if (argc > 1)
		{
			path = argv[1];
			path = path.absolute();
		}
		else
		{
			path = Path::current();
			path = path.pushDirectory("MESH");
		}

		str = path.toString(); 
		MeshToGeo file(str);
		file.GetEleData();
		file.GetNodeData();
		file.CreatGeofile();

		cout << "Num of eles: " << file.NumOfEles() << endl;
		cout << "Num of nodes: " << file.NumOfNodes() << endl;
		cout << "Transformation complemed." << endl;
		return 0;
	}
	catch (Exception& exc)
	{
		cout << exc.displayText() << endl;
		return 1;
	}
}






//---------------------------------------------------------------