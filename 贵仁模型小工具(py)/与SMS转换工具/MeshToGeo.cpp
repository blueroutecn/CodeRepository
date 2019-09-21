
//----------------------------------------------//
//---------------mesh转geo----------------------//
//----------------------------------------------//


#include "MeshToGeo.h"


//***2018.04.24*****************************************


MeshToGeo::MeshToGeo(const string &filepath)
{	
	Path temp(filepath);
	if (!temp.isDirectory())
		temp = temp.parent();

	path = temp.toString(); 
}

MeshToGeo::~MeshToGeo()
{

}

void MeshToGeo::GetEleData()
{
	Path ele_path(path, "ele");
	File ele_file = ele_path.toString();
	if (!ele_file.exists())
		throw	FileExistsException(format(MESHFILE_ERR, ele_path.toString()));


	FileInputStream fis(ele_path.toString());
	DataStreamReader ele(fis, ele_path.toString());
	ele.nextLine();
	

	MESHDATA<int> ele_arr;
	while(!ele.isEnd())
	{
		ele.nextLine();
		if (ele.getDataCount() != 4) continue;

		ele_arr.node1 = ele.getInteger(1);
		ele_arr.node2 = ele.getInteger(2);
		ele_arr.node3 = ele.getInteger(3);

		eles.push_back(ele_arr);
	}
}

void MeshToGeo::GetNodeData()
{
	Path node_path(path, "node");
	File node_file = node_path.toString();
	if (!node_file.exists())
		throw	FileExistsException(format(MESHFILE_ERR, node_path.toString()));

	Path bed_path(path, "bed");
	File bed_file = bed_path.toString();
	if (!bed_file.exists())
		throw	FileExistsException(format(MESHFILE_ERR, bed_path.toString()));


	FileInputStream node_fis(node_path.toString());
	DataStreamReader node(node_fis, node_path.toString());
	node.nextLine();

	XY nodes;
	while (!node.isEnd())
	{
		node.nextLine();
		if (node.getDataCount() < 1) continue;

		nodes.x = node.getDouble(1);
		nodes.y = node.getDouble(2);
		nodes_xy.push_back(nodes);
	}


	FileInputStream bed_fis(bed_path.toString());
	DataStreamReader bed(bed_fis, bed_path.toString());
	bed.nextLine();

	if (bed.getDataCount() > 0)
		nodes_z.push_back(bed.getDouble(0));

	while (!bed.isEnd())
	{
		bed.nextLine();
		if (bed.getDataCount() < 1) continue;

		nodes_z.push_back(bed.getDouble(0));
	}
}

void MeshToGeo::CreatGeofile()
{
	Path temp(path, "geo.geo");
	FileOutputStream geo(temp.toString());


	if (nodes_xy.size() != nodes_z.size())
		throw 
		DataException(format(DATA_ERR, (int)nodes_xy.size(), (int)nodes_z.size()));


	//文件表头。
	geo << "T1" << LINEFEED;
	geo << "T2" << LINEFEED;
	geo << "T3" << LINEFEED;
	geo << "SI" << setw(10) << 0 << LINEFEED;
	geo << "$L" << LINEFEED;
	
	for (size_t i = 0; i < eles.size(); i++)
	{
		//输出每行第1~2项“GE [eleID]”。
		geo << "GE" << setw(10) << i+1 << setw(10);

		//输出每行第3~8项“[node1ID] [midpoint1ID] ……”。
		geo << eles[i].node1 << setw(10) << 0 << setw(10) << eles[i].node2;
		geo << setw(10) << 0 << setw(10) << eles[i].node3 << setw(10) << 0 << setw(10);

		//输出每行第9~12项（一些单元属性）。
		geo << 0 << setw(10) << 0 << setw(10) << 1 << setw(10) << 0.0 << LINEFEED;
	}

	for (size_t i = 0; i < nodes_xy.size(); i++)
	{
		//输出每行第1~2项“GNN [nodeID]”。
		geo << "GNN" << setw(10) << i+1 << setw(20);

		//输出每行第3~4项“[X]  [Y]”。
		//避免以科学计数法输出，否则SMS不能识别。
		geo << setprecision(4) << setiosflags(ios::fixed) <<nodes_xy[i].x << setw(20) << nodes_xy[i].y ;
		//输出每行第5项“[Z]”。
		geo << setw(20)<< setprecision(4) << setiosflags(ios::fixed) << nodes_z[i] << LINEFEED;
	}
}

int MeshToGeo::NumOfEles()
{
	return (int)eles.size();
}

int MeshToGeo::NumOfNodes()
{
	return (int)nodes_xy.size();
}

//---------------------------------------------------------------