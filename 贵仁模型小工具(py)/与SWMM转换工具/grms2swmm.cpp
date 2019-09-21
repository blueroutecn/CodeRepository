//**** head files ***************************************************************
#include "grms2swmm.h"

//*** grms2swmm namespace *******************************************************
using namespace grms2swmm;


//*** class GrmsReader **********************************************************
GrmsReader::GrmsReader()
{
	Path temp = Path::current();
	if (Path::find(temp.toString(), "grms", temp))
		_grmsPath = temp.toString();
	else
		throw DataException(format(SS_ERR1, temp.toString()));
}


GrmsReader::GrmsReader(const string& grms) :_grmsPath(AbsolutePath(grms)){}


void GrmsReader::ReadGrmsData()
{
	ReadHdFile();
	ReadLocationFile();
	ReadJunctionFile();
	ReadManningFile();
	ReadBcFiles();

	GenerateNewCross(_junctions);
	ConvertManning(_manning);
}


void GrmsReader::ReadHdFile()
{
	Path path(_grmsPath);
	Path::find(path.toString(), "Topo", path);
	Path::find(path.toString(), "HD.inp", path);

	FileInputStream fis(path.toString());
	DataStreamReader read(fis, path.toString());

	int serial = 0;                                   // serial num of reaches.
	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		if (read.getString(0).at(0) == '[')
		{			
			REACH reach;			                  // obtain _reaches.
			
			reach.ID = "reach" + num2str(++serial);   // can't extract original reach name, so rename it by num.
			read.nextLine();
			reach.setNum = read.getInteger(0);			
			read.nextLine();

			for (int i = 0; i < reach.setNum; i++)
			{
				TRANSECT tran;                        // obtain _transects.
				tran.ID     = read.getString(1);
				tran.setNum = read.getInteger(0);
				reach.sets.push_back(tran.ID);        // obtain transects' name for a reach.

				vector<STATION> stations;             // obtain stations for a ransect.
				for (int j = 0; j < tran.setNum; j++)
				{
					STATION xy;

					read.nextLine(); 
					xy.x = read.getDouble(0);
					xy.y = read.getDouble(1);
					stations.push_back(xy);
				}
				tran.sets = stations;
				_transects.push_back(tran);
				read.nextLine();
			}
			_reaches.push_back(reach);
		}
	}
}


void GrmsReader::ReadLocationFile()
{
	Path path(_grmsPath);
	Path::find(path.toString(), "Topo", path);
	Path::find(path.toString(), "CrossSectionLocation.inp", path);

	FileInputStream fis(path.toString());
	DataStreamReader read(fis, path.toString());

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1)continue;
		
		XY<double> xy;
		xy.x = read.getDouble(1);
		xy.y = read.getDouble(2);

		COORDINATE coo;
		coo.ID     = read.getString(0);
		coo.setNum = 1;
		coo.sets.push_back(xy);
		_coordinates.push_back(coo);
	}
}


void GrmsReader::ReadJunctionFile()
{
	Path path(_grmsPath);
	Path::find(path.toString(), "Topo", path);
	Path::find(path.toString(), "Junction.inp", path);

	FileInputStream fis(path.toString());
	DataStreamReader read(fis, path.toString());

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		if (read.getString(0) == "[Junctions]")
		{
			read.nextLine();

			while (read.getDataCount() > 0)
			{
				JUNCTION jun;
				jun.ID     = read.getString(0);
				jun.setNum = read.getDataCount() - 1;
				for (int i = 1; i <= jun.setNum; i++)
					jun.sets.push_back(read.getString(i));
				
				_junctions.push_back(jun);
				read.nextLine();
			}
			break;
		}
	}
}


void GrmsReader::ReadManningFile()
{
	Path path(_grmsPath);
	Path::find(path.toString(), "Setting", path);
	Path::find(path.toString(), "Manning.inp", path);

	FileInputStream fis(path.toString());
	DataStreamReader read(fis, path.toString());

	while (!read.isEnd())
	{
		read.nextLine();
		if (read.getDataCount() < 1) continue;

		if (read.getString(0) == "[uniform]")
		{
			read.nextLine();
			_manning.uniform = read.getDouble(0);
			continue;
		}

		if (read.getString(0) == "[segment]")
		{
			read.nextLine();
			int num = read.getInteger(0);

			vector<SEGMENT> segment;
			for (int i = 0; i < num; i++)
			{
				read.nextLine();

				SEGMENT seg;
				seg.dm1 = read.getString(0);
				seg.dm2 = read.getString(1);
				seg.rough = read.getDouble(2);
				segment.push_back(seg);
			}

			_manning.segment = segment;
			continue;
		}

		if (read.getString(0) == "[single]")
		{
			SINGLE sin;

			read.nextLine();
			sin.ID = read.getString(0);
			sin.setNum = read.getInteger(1);

			for (int i = 0; i < sin.setNum; i++)
			{
				STATION xy;

				read.nextLine();
				xy.x = read.getDouble(0);       // x-coor of bank-station.
				xy.y = read.getDouble(2);       // roughness of bank.

				sin.sets.push_back(xy);
			}

			_manning.single.push_back(sin);
		}
	}	
}


void GrmsReader::ReadBcFiles()
{
	Path path1(_grmsPath);
	Path::find(path1.toString(), "BC", path1);
	Path::find(path1.toString(), "bcCrossSections.inp", path1);

	FileInputStream fis1(path1.toString());
	DataStreamReader read1(fis1, path1.toString());

	read1.skipLines(2);
	int Nbc = read1.getInteger(0);  // total num of bc transects.

	read1.skipLines(2);   
	for (int i = 0; i < Nbc; i++, read1.nextLine())
	{
		BC bc;                     // obtain bc, without time series.
		bc.ID = read1.getString(1);
		_bcSeries.push_back(bc);
	}	

	Path path2(_grmsPath);
	Path::find(path2.toString(), "BC", path2);
	Path::find(path2.toString(), "bcTimeSeries.inp", path2);

	FileInputStream fis2(path2.toString());
	DataStreamReader read2(fis2, path2.toString());

	read2.skipLines(2);
	int Lseries = read2.getInteger(0);

	read2.skipLines(2);
	for (int j = 0; j < Lseries; j++, read2.nextLine())
	{
		XY<double> xy;
		xy.x = read2.getDouble(0);

		for (int k = 0; k < Nbc; k++)            // each bc transect has a time series, and its order same to timeseries.
		{
			xy.y = read2.getDouble(k + 1);
			_bcSeries[k].sets.push_back(xy);     // obtain bc transect's time series.
		}
	}

	for (int k = 0; k < Nbc; k++)
	{
		_bcSeries[k].setNum = (int)_bcSeries[k].sets.size();  // obtain bc transect's time series's length.
	}
}


const string GrmsReader::AbsolutePath(const string &grms)
{
	CheckFileExists(grms);

	Path path(grms);
	return path.toString();
}


void GrmsReader::CheckFileExists(const string& file)
{
	File swmmFile(file);
	if (!swmmFile.exists())
		throw FileExistsException(format(SS_ERR1, file));
}


void GrmsReader::GenerateNewCross(const vector<JUNCTION>& junctions)
{
	int serial = 0;                      // serial number for rename cross node.
	vector<JUNCTION>::const_iterator iter;
	for (iter = junctions.begin(); iter != junctions.end(); iter++)
	{
		JUNCTION jun = *iter;
		jun.ID = "cross" + num2str(++serial);
		jun.sets = (*iter).sets;

		_newCrosses.push_back(jun);      
	}
}


void GrmsReader::ConvertManning(const MANNING &_manning)
{
	vector<DM> transects;                  // get all transects with original order.		
	GenerateDM(transects);

	ConvertSingle(transects);

	ConvertSegment(transects);

	ConvertUniform(transects);
}


void GrmsReader::GenerateDM(vector<DM> &transects)
{
	vector<REACH>::const_iterator rea; 
	for (rea = _reaches.begin(); rea != _reaches.end(); rea++)
	{
		vector<string>::const_iterator set;
		for (set = (*rea).sets.begin(); set != (*rea).sets.end(); set++)
		{
			DM dm;
			dm.ID = *set;
			transects.push_back(dm);
		}
	}
}


void GrmsReader::ConvertSingle(vector<DM> &transects)
{
	_roughness = _manning.single;          // get data in [single], final roughness for a trasect.
	vector<SINGLE>::iterator mann;
	for (mann = _roughness.begin(); mann != _roughness.end(); mann++)
	{
		int setN = (*mann).setNum;
		string name = (*mann).ID;
		
		size_t index = FindDataIndex(name,transects);	
		transects[index].mark = YES;       // turn transects' status to YES, for its roughness assigned.
	
		if (setN < 4) continue;

		vector<STATION> newSets;
		STATION xy1;                       // left bank.
		xy1.x = (*mann).sets.front().x;
		xy1.y = (*mann).sets.front().y;
		newSets.push_back(xy1);

		STATION xy2;                       // right bank.
		xy2.x = (*mann).sets[setN - 2].x;
		xy2.y = (*mann).sets.back().y;
		newSets.push_back(xy2);

		double totalRough = 0;
		for (int i = 1; i < setN - 1; i++)
		{
			totalRough += (*mann).sets[i].y;
		}

		STATION xy3;                       // channel.
		xy3.x = (*mann).sets.back().x;
		xy3.y = totalRough / (setN - 2);
		newSets.push_back(xy3);
		
		(*mann).sets = newSets;
	}
}


void GrmsReader::ConvertSegment(vector<DM> &transects)
{
	vector<SEGMENT>::const_iterator iter1; // get data in [segment].
	for (iter1 = _manning.segment.begin(); iter1 != _manning.segment.end(); iter1++)
	{
		string tran1 = (*iter1).dm1;
		string tran2 = (*iter1).dm2;       // dm1,dm2 both located in one same reach. 
		double rough = (*iter1).rough;

		vector<DM>::iterator dm = find_if(transects.begin(), transects.end(), [&tran1](DM s) {return s.ID == tran1; });
		for (; dm != transects.end(); dm++)// start from dm1 to dm2.
		{
			if ((*dm).mark == YES) continue;

			string name = (*dm).ID;
			size_t index = FindDataIndex(name, _transects);

			SINGLE single;
			single.ID = name;
			single.setNum = 1;

			STATION xy;
			xy.x = _transects[index].sets.back().x;
			xy.y = rough;
			single.sets.push_back(xy);

			(*dm).mark = YES;
			_roughness.push_back(single);

			if (name == tran2) break;
		}
	}
}


void GrmsReader::ConvertUniform(vector<DM> &transects)
{
	double rough = _manning.uniform;

	vector<DM>::iterator iter;
	for (iter = transects.begin(); iter != transects.end(); iter++)
	{
		if ((*iter).mark == YES) continue;

		string name = (*iter).ID;
		size_t index = FindDataIndex(name, _transects);

		SINGLE single;
		single.ID = name;
		single.setNum = 1;

		STATION xy;
		xy.x = _transects[index].sets.back().x;
		xy.y = rough;
		single.sets.push_back(xy);

		(*iter).mark = YES;
		_roughness.push_back(single);
	}
}


const vector<REACH>& GrmsReader::ReturnReach()const
{
	CheckDataExists(_reaches);
	return _reaches;
}


const vector<TRANSECT>& GrmsReader::ReturnTransect()const
{
	CheckDataExists(_transects);
	return _transects;
}


const vector<COORDINATE>& GrmsReader::ReurnCoordinate()const
{
	CheckDataExists(_coordinates);
	return _coordinates;
}


const vector<SINGLE>& GrmsReader::ReturnRoughness()const
{
	CheckDataExists(_roughness);
	return _roughness;
}


const vector<JUNCTION>& GrmsReader::ReturnJunction()const
{
	CheckDataExists(_junctions);
	return _junctions;
}


const vector<JUNCTION>& GrmsReader::ReturnNewCross()const
{
	CheckDataExists(_newCrosses);
	return _newCrosses;
}


const vector<BC>& GrmsReader::ReturnBC()const
{
	CheckDataExists(_bcSeries);
	return _bcSeries;
}


//*** class GrmsToNode **********************************************************
GrmsToNode::GrmsToNode(const GrmsReader& reader):_reader(reader){}


void GrmsToNode::GenerateSwmmNode()
{
	const vector<TRANSECT>&   transects = _reader.ReturnTransect();
	const vector<JUNCTION>&   junctions = _reader.ReturnJunction();
	const vector<COORDINATE>& coordinates = _reader.ReurnCoordinate();
	const vector<REACH>&      reaches = _reader.ReturnReach();
	const vector<BC>&         bc = _reader.ReturnBC();

	GenerateTimeseries(bc);
	GenerateJunction(transects);
	GenerateCoordinate(coordinates);
	GenerateOutfall(junctions, reaches,bc);
	GenerateInflow(reaches,bc);

	CreateCrossCoor();
	RemoveDuplicateJunction();
}


void GrmsToNode::GenerateTimeseries(const vector<BC>& bc)
{
	vector<BC>::const_iterator it = bc.begin();
	for (int num = 0; it != bc.end(); it++,num++)
	{
		swmmTimeseries series;
		series.ID = "xl" + num2str(num + 1);      // name time series from 1;
		series.series = (*it).sets;
		_swmmTimeseries.push_back(series);
	}
}


void GrmsToNode::GenerateJunction(const vector<TRANSECT>& transects)
{	
	vector<TRANSECT>::const_iterator iter;
	for (iter = transects.begin(); iter != transects.end(); iter++)
	{
		swmmJUNCTION swmm;
		swmm.ID = (*iter).ID;              // use transect's id as swmm junction's id.
		swmm.elevation = (*std::min_element((*iter).sets.begin(), (*iter).sets.end(),
			                    [](STATION s1, STATION s2) { return s1.y < s2.y; })).y; // use min y-coord as elevation.
		swmm.maxDepth = swmm.initDepth = swmm.surDepth = swmm.aponded = 0;
		_swmmJunctions.push_back(swmm);
	}
}


void GrmsToNode::GenerateCoordinate(const vector<COORDINATE>& coordinates)
{
	vector<COORDINATE>::const_iterator iter;
	for (iter = coordinates.begin(); iter != coordinates.end(); iter++)
	{
		swmmCOORDINATE coor;
		coor.ID = (*iter).ID;
		coor.xy = (*iter).sets[0];       // vector<XY> sets only has one (x,y).
		_swmmCoordinates.push_back(coor);
	}
}


void GrmsToNode::GenerateOutfall(const vector<JUNCTION>& junctions, const vector<REACH>& reaches, const vector<BC>& bc)
{
	vector<string> crosses;
	vector<JUNCTION>::const_iterator jun;
	for (jun = junctions.begin(); jun != junctions.end(); jun++)
	{
		vector<string>::const_iterator set;
		for (set = (*jun).sets.begin(); set != (*jun).sets.end(); set++)
		{
			crosses.push_back(*set);
		}
	}

	vector<REACH>::const_iterator rea;
	for (rea = reaches.begin(); rea != reaches.end(); rea++)
	{
		string endNode = (*rea).sets.back();

		if (find(begin(crosses), end(crosses), endNode) == crosses.end()) // if it's endnode & non-cross,it's outfall.
		{
			swmmOUTFALL outfall;                           // obtain _swmmOutfalls.
			outfall.ID = endNode;

			size_t bcN = find_if(begin(bc), end(bc), [&endNode](BC obj) {return obj.ID == endNode; }) - bc.begin();
			if (bcN < bc.size())
			{
				outfall.type = "TIMESERIES";
				outfall.stage = "xl" + num2str(bcN + 1);   // time series of outfall, name it from 1.
			}
			
			size_t junN = _reader.FindDataIndex(endNode, _swmmJunctions); 
			outfall.elevation = _swmmJunctions[junN].elevation;

			_swmmOutfalls.push_back(outfall);
		}
	}
}


void GrmsToNode::GenerateInflow(const vector<REACH>& reaches, const vector<BC>& bc)
{
	swmmINFLOW inflow;

	// use upnode of first transect of first reach as inflow node.
	string upNode = (reaches.front()).sets.front();
	inflow.ID = upNode;

	size_t index = _reader.FindDataIndex(upNode, bc); 
	inflow.series = "xl" + num2str(index + 1);      // name of series, name it from 1.
	
	_swmmInflows.push_back(inflow);
}


void GrmsToNode::CreateCrossCoor()
{
	const vector<JUNCTION>& crosses = _reader.ReturnNewCross();

	vector<JUNCTION>::const_iterator iter;
	for (iter = crosses.begin(); iter != crosses.end(); iter++)
	{
		swmmJUNCTION jun;                // to create swmmJunction for newly created cross node.
		jun.ID = (*iter).ID;
		jun.elevation = CreateZ(*iter);  
		jun.maxDepth = jun.initDepth = jun.surDepth = jun.aponded = 0;
		_swmmJunctions.push_back(jun); 

		swmmCOORDINATE coor;             // to create swmmCoordinate for newly created cross node.
		coor.ID = (*iter).ID;
		CreateXY(*iter,coor.xy);        
		_swmmCoordinates.push_back(coor);
	}
}


void GrmsToNode::RemoveDuplicateJunction()
{
	const vector<JUNCTION>& crosses = _reader.ReturnNewCross();

	// remove cross nodes from _swmmJunctions.
	vector<JUNCTION>::const_iterator jun;
	for (jun = crosses.begin(); jun != crosses.end(); jun++)
	{
		vector<string>::const_iterator set;
		for (set = (*jun).sets.begin(); set != (*jun).sets.end(); set++)
		{
			string id = *set;

			remove_if(_swmmJunctions.begin(), _swmmJunctions.end(),
				[&id](swmmJUNCTION obj) {return (obj.ID == id) ? TRUE : FALSE; });

			remove_if(begin(_swmmCoordinates), end(_swmmCoordinates),
				[&id](swmmCOORDINATE obj) {return (obj.ID == id) ? TRUE : FALSE; });
		}
	}

	// remove outfalls from _swmmJunctions.
	vector<swmmOUTFALL>::const_iterator out;
	for (out = _swmmOutfalls.begin(); out != _swmmOutfalls.end(); out++)
	{
		string id = (*out).ID;

		remove_if(_swmmJunctions.begin(), _swmmJunctions.end(),
			[&id](swmmJUNCTION obj) {return (obj.ID == id) ? TRUE : FALSE; });
	}

	// remove_if() can't really erase element, so finish it here.
	vector<swmmJUNCTION>::iterator swmmJun;
	for (swmmJun = _swmmJunctions.begin(); swmmJun != _swmmJunctions.end();)
	{
		((*swmmJun).ID == "") ? swmmJun = _swmmJunctions.erase(swmmJun) : swmmJun++;
	}

	vector<swmmCOORDINATE>::iterator swmmCoo;
	for (swmmCoo = _swmmCoordinates.begin(); swmmCoo != _swmmCoordinates.end();)
	{
		((*swmmCoo).ID == "") ? swmmCoo = _swmmCoordinates.erase(swmmCoo) : swmmCoo++;
	}
}


double GrmsToNode::CreateZ(const JUNCTION& jun)
{
	_reader.CheckDataExists(_swmmJunctions);

	double totalZ = 0;
	for (int i = 0; i < jun.setNum; i++)
	{
		size_t n = _reader.FindDataIndex(jun.sets[i],_swmmJunctions);
		totalZ += _swmmJunctions[n].elevation;
	}

	return totalZ / jun.setNum;        // return averaging z-coor for cross node.
}


XY<double> GrmsToNode::CreateXY(const JUNCTION& jun, XY<double>& xy)
{
	_reader.CheckDataExists(_swmmCoordinates);

	double totalX = 0;
	double totalY = 0;
	for (int i = 0; i < jun.setNum; i++)
	{
		size_t n = _reader.FindDataIndex(jun.sets[i], _swmmCoordinates);
		totalX += _swmmCoordinates[n].xy.x;
		totalY += _swmmCoordinates[n].xy.y;
	}

	xy.x = totalX / jun.setNum;
	xy.y = totalY / jun.setNum;

	return xy;
}


const vector<swmmTimeseries>& GrmsToNode::ReturnSwmmTimeseries()const
{
	_reader.CheckDataExists(_swmmTimeseries);
	return _swmmTimeseries;
}


const vector<swmmJUNCTION>& GrmsToNode::ReturnSwmmJunction()const
{
	_reader.CheckDataExists(_swmmJunctions);
	return _swmmJunctions;
}


const vector<swmmOUTFALL>& GrmsToNode::ReturnSwmmOutfall()const
{
	_reader.CheckDataExists(_swmmOutfalls);
	return _swmmOutfalls;
}


const vector<swmmINFLOW>& GrmsToNode::ReturnSwmmInflow()const
{
	_reader.CheckDataExists(_swmmInflows);
	return _swmmInflows;
}


const vector<swmmCOORDINATE>& GrmsToNode::ReturnSwmmCoordinate()const
{
	_reader.CheckDataExists(_swmmCoordinates);
	return _swmmCoordinates;
}


//*** class GrmsToLink **********************************************************
GrmsToLink::GrmsToLink(const GrmsReader& reader) :_reader(reader){}


void GrmsToLink::GenerateSwmmLink()
{
	GenerateConduit();
	ResetJunction();
}


void GrmsToLink::GenerateConduit()
{
	const vector<REACH>&      reach = _reader.ReturnReach();
	const vector<COORDINATE>& coordinate = _reader.ReurnCoordinate();

	int serial = 0;                      // serial num for conduits & transects.
	vector<REACH>::const_iterator iter;
	for (iter = reach.begin(); iter != reach.end(); iter++)
	{
		vector<string>::const_iterator set;
		for (set = (*iter).sets.begin(); set != (*iter).sets.end()-1; set++)
		{
			serial++;

			swmmCONDUIT conduit;         // obtain _swmmConduit without roughness which is provided by transect.
			conduit.ID       = "GQ" + num2str(serial);
			conduit.upNode   = *set;
			conduit.downNode = *(set + 1);

		    size_t  upNum = _reader.FindDataIndex(conduit.upNode, coordinate);
			size_t  downNum = _reader.FindDataIndex(conduit.downNode, coordinate);
			conduit.length = length(upNum, downNum);
			_swmmConduits.push_back(conduit);

			swmmSECTION section;         // obtain _swmmSection.
			section.ID = conduit.ID;
			section.geom1 = "DM" + num2str(serial);
			_swmmSections.push_back(section);
		}
	}
	
	ResetJunction();
}


void GrmsToLink::ResetJunction()
{
	const vector<JUNCTION>& crosses = _reader.ReturnNewCross();

	vector<JUNCTION>::const_iterator cross;
	for (cross = crosses.begin(); cross != crosses.end(); cross++)
	{
		string crossName = (*cross).ID;         // new cross.

		vector<string>::const_iterator set;
		for (set = (*cross).sets.begin(); set != (*cross).sets.end(); set++)
		{
			string junName = *set;             // old junction, replace it with new cross.
			size_t size = _swmmConduits.size();

			size_t upNode = find_if(begin(_swmmConduits), end(_swmmConduits),
				         [&junName](swmmCONDUIT con) {return(con.upNode == junName) ? TRUE : FALSE; }) - _swmmConduits.begin();

			if (upNode != size)
			{
				_swmmConduits[upNode].upNode = crossName;
				continue;                      // nodes of a conduit come from old transects, each one appears only once. 
			}

			size_t downNode = find_if(begin(_swmmConduits), end(_swmmConduits),
				           [&junName](swmmCONDUIT con) {return(con.downNode == junName) ? TRUE : FALSE; }) - _swmmConduits.begin();
		
			if (downNode != size)
			{
				_swmmConduits[downNode].downNode = crossName;
				continue;
			}
		}
	}
}


const vector<swmmCONDUIT>& GrmsToLink::ReturnSwmmConduit()const
{
	_reader.CheckDataExists(_swmmConduits);
	return _swmmConduits;
}


const vector<swmmSECTION>& GrmsToLink::ReturnSwmmSection()const
{
	_reader.CheckDataExists(_swmmSections);
	return _swmmSections;
}



//*** class GrmsToTransect ******************************************************
GrmsToTransect::GrmsToTransect(const GrmsReader& reader):
	_reader(reader),_station(MaxStation()),_newTransects(_reader.ReturnTransect()){}


int GrmsToTransect::MaxStation()
{
	const vector<TRANSECT>& transect = _reader.ReturnTransect();

	int station = (*max_element(begin(transect), end(transect), 
		            [](TRANSECT t1, TRANSECT t2) {return t1.setNum < t2.setNum; })).setNum;

	return station;
}


void GrmsToTransect::ResampleTransect()
{
	vector<TRANSECT>::iterator tran;
	for (tran = _newTransects.begin(); tran != _newTransects.end(); tran++)
	{		
		double width = 0;
		vector<XY<double>> XL;
		ConnectXtoL(*tran, width, XL);                

		vector<STATION> newXY;
		double length = width / _station;              // Resample transect, based on min-length(width).

		for (int i = 0; i <= _station; i++)
		{
			if (i == 0)                                // Start point of the transect.
			{
				STATION xy;
				xy.x = (*tran).sets[0].x;
				xy.y = (*tran).sets[0].y;
				newXY.push_back(xy);
				continue;
			}
			else if (i == _station)                    // End point of the transect.
			{
				STATION xy;
				int num = (*tran).setNum;
				xy.x = (*tran).sets[num - 1].x;
				xy.y = (*tran).sets[num - 1].y;
				newXY.push_back(xy);
				break;
			}

			STATION xy;
			double l = i * length;
			CreateStation(l, xy, XL, *tran);

			newXY.push_back(xy);
		}

		(*tran).setNum = (int)newXY.size();
		(*tran).sets = newXY;
	}
}


void GrmsToTransect::ConnectXtoL(const TRANSECT &tran, double &width, vector<XY<double>> &xl)
{
	STATION xy;
	xy.x = xy.y = 0;
	xl.push_back(xy);            // first station.

	vector<STATION>::const_iterator iter;
	for (iter = tran.sets.begin(); iter != tran.sets.end() - 1; iter++)
	{
		STATION temp;
		double diffX = (*(iter + 1)).x - (*iter).x;
		double diffY = (*(iter + 1)).y - (*iter).y;
		double length = sqrt(diffX*diffX + diffY * diffY);
		width += length;

		temp.x = (*(iter + 1)).x;
		temp.y = width;
		xl.push_back(temp);
	}
}


void GrmsToTransect::CreateStation(double l, STATION &station, const vector<STATION> &xl, const TRANSECT &tran)
{
	vector<STATION>::const_iterator iter;
	for (iter = xl.begin(); iter != xl.end() - 1; iter++)
	{
		if (l >= (*iter).y && l <= (*(iter + 1)).y) break;     // Find the line segment that l located.
	}

	if (iter == (xl.end() - 1))
		throw DataException(format(SS_ERR3, tran.ID));


	size_t xyNum = iter - xl.begin();                         // Location in xl array same to in the xy array.
	double x1, y1, x2, y2;
	x1 = tran.sets[xyNum].x;      y1 = tran.sets[xyNum].y;
	x2 = tran.sets[xyNum + 1].x;  y2 = tran.sets[xyNum + 1].y;

	double x, y;                                               // Turn l to (x,y).
	double diffL = l - xl[xyNum].y;                            // Relative l.

	if (fabs(x1 - x2) < __DELTA)                               
	{
		x = x1;

		double a = 1;
		double b = -2 * y1;
		double c = pow(y1, 2) - pow(diffL, 2);

		if (y1>y2)
			y = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
		else
			y = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);

		if (fabs(y) < __DELTA)  y = 0;
	}
	else
	{
		double k = (y2 - y1) / (x2 - x1);

		x = x1 + diffL / sqrt(k*k + 1);
		y = k * (x - x1) + y1;

		if (fabs(y) < __DELTA)  y = 0;
	}

	station.x = x;
	station.y = y;
}


void GrmsToTransect::GenerateSwmmTransect()
{
	ResampleTransect();

	const vector<REACH>&    reach = _reader.ReturnReach();
	const vector<TRANSECT>& transects = _reader.ReturnTransect();
	const vector<SINGLE>&   roughness = _reader.ReturnRoughness();

	int serial = 0;                       
	vector<REACH>::const_iterator iter;
	for (iter = reach.begin(); iter != reach.end(); iter++)
	{
		vector<string>::const_iterator set;
		for (set = (*iter).sets.begin(); set != (*iter).sets.end() - 1; set++)
		{
			swmmTRANSECT transect;                  // obtain _swmmTransects.
			transect.ID = "DM" + num2str(++serial);

			size_t rough1 = _reader.FindDataIndex(*set, roughness);
			size_t rough2 = _reader.FindDataIndex(*(set + 1), roughness);
			GenerateNCandX1(rough1, rough2, transect);
			
			size_t tran1 = _reader.FindDataIndex(*set, transects);
			size_t tran2 = _reader.FindDataIndex(*(set + 1), transects);
			GenerateSwmmStation(tran1,tran2, transect);
			
			_swmmTransects.push_back(transect);
		}
	}
}


void GrmsToTransect::GenerateNCandX1(size_t rough1, size_t rough2, swmmTRANSECT &transect)
{
	const vector<SINGLE>& roughness = _reader.ReturnRoughness();
	const SINGLE &single1 = roughness[rough1];
	const SINGLE &single2 = roughness[rough2];

	double Xleft, Nleft, Xright, Nright, Xchan, Nchan;

	NC temp1;
	X1 temp2;
	temp2.ID = transect.ID;
	temp2.Nstation = _station;
	
	if (single1.setNum == 3 && single2.setNum == 3)
	{
		Xleft  = (single1.sets[0].x + single2.sets[0].x) / 2;
		Xright = (single1.sets[1].x + single2.sets[1].x) / 2;
		Xchan  = (single1.sets[2].x + single2.sets[2].x) / 2;

		Nleft  = (single1.sets[0].y + single2.sets[0].y) / 2;
		Nright = (single1.sets[1].y + single2.sets[1].y) / 2;
		Nchan  = (single1.sets[2].y + single2.sets[2].y) / 2;

		temp1.Nchanl = Nchan;
		temp1.Nleft = Nleft;
		temp1.Nright = Nright;

		temp2.Xleft = Xleft;
		temp2.Xright = Xright;

		transect.rough = temp1;
		transect.param = temp2;
		return;
	}

	if (single1.setNum == 2 && single2.setNum == 3)
	{
		if (single1.sets[0].x > (single1.sets[1].x - single1.sets[0].x))  // right and channel.
		{
			Xright = (single1.sets[0].x + single2.sets[1].x) / 2;
			Xchan  = (single1.sets[1].x + single2.sets[2].x) / 2;

			Nright = (single1.sets[0].y + single2.sets[1].y) / 2;
			Nchan  = (single1.sets[1].y + single2.sets[2].y) / 2;
			
			temp1.Nright = Nright;
			temp1.Nchanl = Nchan;
			
			temp2.Xright = Xright;

			transect.rough = temp1;
			transect.param = temp2;
			return;
		}
		else                                                              // left and channel.
		{
			Xleft = (single1.sets[0].x + single2.sets[0].x) / 2;
			Xchan = (single1.sets[1].x + single2.sets[2].x) / 2;

			Nleft = (single1.sets[0].y + single2.sets[0].y) / 2;
			Nchan = (single1.sets[1].y + single2.sets[2].y) / 2;

			temp1.Nleft = Nleft;
			temp1.Nchanl = Nchan;

			temp2.Xleft = Xleft;

			transect.rough = temp1;
			transect.param = temp2;
			return;
		}
	}
	
	if (single1.setNum == 3 && single2.setNum == 2)
	{
		if (single1.sets[0].x > (single1.sets[1].x - single1.sets[0].x))  // right and channel.
		{
			Xright = (single1.sets[1].x + single2.sets[0].x) / 2;
			Xchan = (single1.sets[2].x + single2.sets[1].x) / 2;

			Nright = (single1.sets[1].y + single2.sets[0].y) / 2;
			Nchan = (single1.sets[2].y + single2.sets[1].y) / 2;

			temp1.Nright = Nright;
			temp1.Nchanl = Nchan;

			temp2.Xright = Xright;

			transect.rough = temp1;
			transect.param = temp2;
			return;
		}
		else                                                              // left and channel.
		{
			Xleft = (single1.sets[0].x + single2.sets[0].x) / 2;
			Xchan = (single1.sets[2].x + single2.sets[1].x) / 2;

			Nleft = (single1.sets[0].y + single2.sets[0].y) / 2;
			Nchan = (single1.sets[2].y + single2.sets[1].y) / 2;

			temp1.Nleft = Nleft;
			temp1.Nchanl = Nchan;

			temp2.Xleft = Xleft;

			transect.rough = temp1;
			transect.param = temp2;
			return;
		}
	}

	transect.rough = temp1;
	transect.param = temp2;	
}


void GrmsToTransect::GenerateSwmmStation(size_t tran1, size_t tran2, swmmTRANSECT &transect)
{
	const vector<TRANSECT>& transects = _reader.ReturnTransect();
	
	vector<STATION> newXY;
	for (int i = 0; i < _station; i++)
	{
		STATION xy;
		double x1, y1, x2, y2;

		x1 = transects[tran1].sets[i].x;
		y1 = transects[tran1].sets[i].y;
		x2 = transects[tran2].sets[i].x;
		y2 = transects[tran2].sets[i].y;

		xy.x = (x1 + x2) /2;
		xy.y = (y1 + y2) /2;
		newXY.push_back(xy);
	}
	
	if (transect.param.Xleft != 0 || transect.param.Xright != 0)
		GenerateBankStation(transect, newXY);
	
	transect.stations = newXY;
	transect.param.Nstation = (int)newXY.size();
}


void  GrmsToTransect::GenerateBankStation(const swmmTRANSECT &transect, vector<STATION> &newXY)
{
	size_t leftNum = -1;
	size_t rightNum = -1;

	// judge whether left and right bank exists in resample-transects.
	vector<STATION>::const_iterator iter1;
	for (iter1 = newXY.begin(); iter1 != newXY.end(); iter1++)
	{
		if (fabs((*iter1).x - transect.param.Xleft) < __DELTA)
		{
			leftNum = iter1 - newXY.begin();
		}
		else if (fabs((*iter1).x - transect.param.Xright) < __DELTA)
		{
			rightNum = iter1 - newXY.begin();		break;
		}
	}

	// left-bank don't exist in resample-transect, and roughness of left-bank exists.
	if (leftNum == -1 && transect.param.Xleft != 0)
	{
		STATION left;

		// locate left-bank interval in resample-transect.
		vector<STATION>::const_iterator iter2;
		for (iter2 = newXY.begin(); iter2 != newXY.end() - 1; iter2++)
		{
			if (transect.param.Xleft >= (*iter2).x && transect.param.Xleft <= (*(iter2 + 1)).x)
			{
				leftNum = iter2 - newXY.begin();		break;
			}
		}

		if (leftNum == -1)
			throw DataException(format(SS_ERR4, transect.ID));

		double x1, y1, x2, y2;
		x1 = newXY[leftNum].x;      y1 = newXY[leftNum].y;
		x2 = newXY[leftNum + 1].x;  y2 = newXY[leftNum + 1].y;
		double k = (y2 - y1) / (x2 - x1);

		left.x = transect.param.Xleft;
		left.y = k * (left.x - x1) + y1;
		newXY.insert(iter2 + 1, left);
	}

	if (rightNum == -1 && transect.param.Xright != 0)
	{
		STATION right;
		vector<STATION>::const_iterator iter2;
		for (iter2 = newXY.begin(); iter2 != newXY.end() - 1; iter2++)
		{
			if (transect.param.Xright >= (*iter2).x && transect.param.Xright <= (*(iter2 + 1)).x)
			{
				rightNum = iter2 - newXY.begin();		break;
			}
		}

		if (rightNum == -1)
			throw DataException(format(SS_ERR4, transect.ID));

		double x1, y1, x2, y2;
		x1 = newXY[rightNum].x;      y1 = newXY[rightNum].y;
		x2 = newXY[rightNum + 1].x;  y2 = newXY[rightNum + 1].y;
		double k = (y2 - y1) / (x2 - x1);

		right.x = transect.param.Xright;
		right.y = k * (right.x - x1) + y1;
		newXY.insert(iter2 + 1, right);
	}
}


const vector<swmmTRANSECT>& GrmsToTransect::ReturnSwmmTransect()const
{
	_reader.CheckDataExists(_swmmTransects);
	return _swmmTransects;
}



//*** class GrmsToSwmm ******************************************************
GrmsToSwmm::GrmsToSwmm(const string &path, const GrmsToNode &node, const GrmsToLink &link, const GrmsToTransect &tran) :
	_swmmPath(path), _node(node), _link(link), _transect(tran){}


GrmsToSwmm::GrmsToSwmm(const GrmsToNode &node, const GrmsToLink &link, const GrmsToTransect &tran):
    _node(node), _link(link), _transect(tran)
{
	Path temp = Path::current();
	if (Path::find(temp.toString(), "swmm.inp", temp))
		_swmmPath = temp.toString();
	else
	{
		temp = temp.toString() + "\\swmm.inp";
		_swmmPath = temp.toString();
	}
}


void GrmsToSwmm::WriteSwmmFile()
{
	Path swmm(_swmmPath);
	FileOutputStream file(swmm.toString());
	
	file.setf(ios::left);
	file.setf(ios::fixed);
	file.precision(4);

	OutTitle(file);
	OutOptions(file);
	OutEvaporation(file);
	OutJunctions(file);
	OutOutfalls(file);
	OutConduits(file);
	OutXsections(file);
	OutTransects(file);
	OutInflows(file);
	OutTimeseries(file);
	OutReport(file);
	OutTags(file);
	OutMap(file);
	OutCoordinates(file);
	OutVertices(file);
	OutProfiles(file);
}


void GrmsToSwmm::OutTitle(FileOutputStream &file)
{
	file.setf(ios::left);
	file << "[TITLE]" << endl;
	file << ";;Project Title/Notes" << endl; file << endl;
}


void GrmsToSwmm::OutOptions(FileOutputStream &file)
{
	file << "[OPTIONS]" << endl;
	file << ";;Option             Value" << endl; 

	file << setw(21) << "FLOW_UNITS "        << setw(15) << "CMS"     << endl;
	file << setw(21) << "INFILTRATION "      << setw(15) << "HORTON"  << endl;
	file << setw(21) << "FLOW_ROUTING "      << setw(15) << "DYNWAVE" << endl;
	file << setw(21) << "LINK_OFFSETS "      << setw(15) << "DEPTH"   << endl;
	file << setw(21) << "MIN_SLOPE "         << setw(15) << "0"       << endl;
	file << setw(21) << "ALLOW_PONDING "     << setw(15) << "NO"      << endl;
	file << setw(21) << "SKIP_STEADY_STATE " << setw(15) << "NO"      << endl;
	file << endl;
				 
	file << setw(21) << "START_DATE "        << setw(15) << "06/04/2018" << endl;
	file << setw(21) << "START_TIME "        << setw(15) << "00:00:00"   << endl;
	file << setw(21) << "REPORT_START_DATE " << setw(15) << "06/04/2018" << endl;
	file << setw(21) << "REPORT_START_TIME " << setw(15) << "00:00:00"   << endl;
	file << setw(21) << "END_DATE "          << setw(15) << "06/05/2018" << endl;
	file << setw(21) << "END_TIME "          << setw(15) << "00:00:00"   << endl;
	file << setw(21) << "SWEEP_START "       << setw(15) << "01/01"      << endl;
	file << setw(21) << "SWEEP_END "         << setw(15) << "12/31"      << endl;
	file << setw(21) << "DRY_DAYS "          << setw(15) << "0"          << endl;
	file << setw(21) << "REPORT_STEP "       << setw(15) << "00:15:00"   << endl;
	file << setw(21) << "WET_STEP "          << setw(15) << "00:05:00"   << endl;
	file << setw(21) << "DRY_STEP "          << setw(15) << "01:00:00"   << endl;
	file << setw(21) << "ROUTING_STEP "      << setw(15) << "0:00:05"    << endl;
	file << endl;
				 
	file << setw(21) << "INERTIAL_DAMPING "    << setw(15) << "PARTIAL" << endl;
	file << setw(21) << "NORMAL_FLOW_LIMITED " << setw(15) << "BOTH"    << endl;
	file << setw(21) << "FORCE_MAIN_EQUATION " << setw(15) << "H-W"     << endl;
	file << setw(21) << "VARIABLE_STEP "       << setw(15) << "0.75"    << endl;
	file << setw(21) << "LENGTHENING_STEP "    << setw(15) << "0"       << endl;
	file << setw(21) << "MIN_SURFAREA "        << setw(15) << "1.14"    << endl;
	file << setw(21) << "MAX_TRIALS "          << setw(15) << "8"       << endl;
	file << setw(21) << "HEAD_TOLERANCE "      << setw(15) << "0.0015"  << endl;
	file << setw(21) << "SYS_FLOW_TOL "        << setw(15) << "5"       << endl;
	file << setw(21) << "LAT_FLOW_TOL "        << setw(15) << "5"       << endl;
	file << setw(21) << "MINIMUM_STEP "        << setw(15) << "0.5"     << endl;
	file << setw(21) << "THREADS "             << setw(15) << "1"       << endl;
	file << endl;
}


void GrmsToSwmm::OutEvaporation(FileOutputStream &file)
{
	file << "[EVAPORATION]" << endl;
	file << ";;Data Source    Parameters" << endl;
	file << ";;-------------- ----------------" << endl;

	file << setw(20) << "CONSTANT " << setw(10) << "0.0" << endl;
	file << setw(20) << "DRY_ONLY " << setw(10) << "NO" << endl;
	file << endl;
}


void GrmsToSwmm::OutJunctions(FileOutputStream &file)
{
	file << "[JUNCTIONS]" << endl;
	file << ";;Name           Elevation  MaxDepth   InitDepth  SurDepth   Aponded " << endl;
	file << ";;-------------- ---------- ---------- ---------- ---------- ----------" << endl;

	const vector<swmmJUNCTION>& junctions = _node.ReturnSwmmJunction();
	vector<swmmJUNCTION>::const_iterator it;
	for (it = junctions.begin(); it != junctions.end(); it++)
	{
		file << setw(17) << (*it).ID << setw(11) << (*it).elevation << setw(11) << (*it).maxDepth;
		file << setw(11) << (*it).initDepth << setw(11) << (*it).surDepth << setw(11) << (*it).aponded << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutOutfalls(FileOutputStream &file)
{
	file << "[OUTFALLS]" << endl;
	file << ";;Name           Elevation  Type       Stage Data       Gated    Route To " << endl;
	file << ";;-------------- ---------- ---------- ---------------- -------- ----------------" << endl;

	const vector<swmmOUTFALL>& outfalls = _node.ReturnSwmmOutfall();
	vector<swmmOUTFALL>::const_iterator it;
	for (it = outfalls.begin(); it != outfalls.end(); it++)
	{
		file << setw(18) << (*it).ID    << setw(10) << (*it).elevation << setw(12) << (*it).type;
		file << setw(16) << (*it).stage << setw(10) << (*it).gated     << setw(15) << (*it).routeTo << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutInflows(FileOutputStream &file)
{
	file << "[INFLOWS]" << endl;
	file << ";;Node           Constituent      Time Series      Type     Mfactor  Sfactor  Baseline Pattern" << endl;
	file << ";;-------------- ---------------- ---------------- -------- -------- -------- -------- --------" << endl;

	const vector<swmmINFLOW>& inflows = _node.ReturnSwmmInflow();
	vector<swmmINFLOW>::const_iterator it;
	for (it = inflows.begin(); it != inflows.end(); it++)
	{
		file << setw(18) << (*it).ID      << setw(17)<< (*it).constituent << setw(16)<< (*it).series;
		file << setw(10) << (*it).type    << setw(9) << (*it).mFactor     << setw(9) << (*it).sFactor;
		file << setw(9)  << (*it).baseLine<< setw(9) << (*it).pattern     << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutCoordinates(FileOutputStream &file)
{
	file << "[COORDINATES]" << endl;
	file << ";;Node           X-Coord            Y-Coord " << endl;
	file << ";;-------------- ------------------ ------------------" << endl;

	const vector<swmmCOORDINATE>& coordinates = _node.ReturnSwmmCoordinate();
	vector<swmmCOORDINATE>::const_iterator it;
	for (it = coordinates.begin(); it != coordinates.end(); it++)
	{
		file << setw(18) << (*it).ID << setw(20) << (*it).xy.x << setw(20) << (*it).xy.y << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutConduits(FileOutputStream &file)
{
	file << "[CONDUITS]" << endl;
	file << ";;Name           From Node        To Node          Length     Roughness  InOffset   OutOffset  InitFlow   MaxFlow " << endl;
	file << ";;-------------- ---------------- ---------------- ---------- ---------- ---------- ---------- ---------- ----------" << endl;

	const vector<swmmCONDUIT>& conduits = _link.ReturnSwmmConduit();
	vector<swmmCONDUIT>::const_iterator it;
	for (it = conduits.begin(); it != conduits.end(); it++)
	{
		file << setw(17) << (*it).ID << setw(17) << (*it).upNode << setw(17) << (*it).downNode << setw(12) << (*it).length;
		file << setw(11) << (*it).roughness << setw(11) << (*it).inOff   << setw(11) << (*it).outOff;
		file << setw(11) << (*it).initFlow  << setw(11) << (*it).maxFlow << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutXsections(FileOutputStream &file)
{
	file << "[XSECTIONS]" << endl;
	file << ";;Link           Shape        Geom1            Geom2      Geom3      Geom4      Barrels    Culvert " << endl;
	file << ";;-------------- ------------ ---------------- ---------- ---------- ---------- ---------- ----------" << endl;

	const vector<swmmSECTION>& sections = _link.ReturnSwmmSection();
	vector<swmmSECTION>::const_iterator it;
	for (it = sections.begin(); it != sections.end(); it++)
	{
		file << setw(17) << (*it).ID    << setw(14) << (*it).shape << setw(16) << (*it).geom1  << setw(11) << (*it).geom2;
		file << setw(11) << (*it).geom3 << setw(11) << (*it).geom4 << setw(11) << (*it).barrel << setw(11) << (*it).culvert << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutTransects(FileOutputStream &file)
{
	file << "[TRANSECTS]" << endl;
	file << ";;Transect Data in HEC-2 format" << endl;

	const vector<swmmTRANSECT>& transects = _transect.ReturnSwmmTransect();
	vector<swmmTRANSECT>::const_iterator tran;
	for (tran = transects.begin(); tran != transects.end(); tran++)
	{
		file << ";" << endl;

		const NC &nc = (*tran).rough;
		file << setw(5) << "NC" << setw(10) << nc.Nleft << setw(10) << nc.Nright << setw(10) << nc.Nchanl << endl;

		const X1 &x1 = (*tran).param;
		file << setw(5) << "X1" << setw(15) << x1.ID << setw(10) << x1.Nstation;

		// to avoid Xright from 0.0 to 0.0000(start station of a transect), which vould make whole channel a rightbank.
		file << setw(10) << x1.Xleft;	
		if (fabs(x1.Xright - 0) < __DELTA) file << setw(10) << "0.0";
		else file << setw(10) << x1.Xright;

		file << setw(10) << "0"  << setw(10) << "0" ;
		file << setw(10) << x1.Lfactor  << setw(10) << x1.Wfactor << setw(10) << x1.Eoffset << endl;
		
		file << setw(5) << "GR";
		vector<STATION>::const_iterator sta = (*tran).stations.begin();
		for (int count =1; sta != (*tran).stations.end(); sta++, count++)
		{
			file << setw(10) << (*sta).y << setw(10) << (*sta).x;		

			if (count % 5 == 0)    // output (x,y) first, then do the judge.( because of count++)
			{
				file << endl;
				file << setw(5) << "GR";
			}
		}

		file << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutTimeseries(FileOutputStream &file)
{
	file << "[TIMESERIES]" << endl;
	file << ";;Name           Date       Time       Value    " << endl;
	file << ";;-------------- ---------- ---------- ----------" << endl;

	const vector<swmmTimeseries>& timeSeries = _node.ReturnSwmmTimeseries();
	vector<swmmTimeseries>::const_iterator it;
	for (it = timeSeries.begin(); it != timeSeries.end(); it++)
	{
		string id = (*it).ID;
		string data = (*it).data;

		vector<XY<double>>::const_iterator xy;
		for (xy = (*it).series.begin(); xy != (*it).series.end(); xy++)
		{
			file << setw(18) << id << setw(10) << data;
			file << setw(12) << (*xy).x << setw(12) << (*xy).y << endl;
		}
		file << ";" << endl;
	}

	file << endl;
}


void GrmsToSwmm::OutReport(FileOutputStream &file)
{
	file << "[REPORT]" << endl;
	file << ";;Reporting Options" << endl;

	file << setw(20) << "INPUT"         << setw(10) << "NO"  << endl;
	file << setw(20) << "CONTROLS"      << setw(10) << "YES" << endl;
	file << setw(20) << "SUBCATCHMENTS" << setw(10) << "ALL" << endl;
	file << setw(20) << "NODES" << setw(10) << "ALL" << endl;
	file << setw(20) << "LINKS" << setw(10) << "ALL" << endl;
	file << endl;
}


void GrmsToSwmm::OutTags(FileOutputStream &file)
{
	file << "[TAGS]" << endl;
	file << endl;
}


void GrmsToSwmm::OutMap(FileOutputStream &file)
{
	file << "[MAP]" << endl;

	const vector<swmmCOORDINATE>& coordinates = _node.ReturnSwmmCoordinate();
	double minX = (*min_element(begin(coordinates), end(coordinates),
		         [](swmmCOORDINATE c1, swmmCOORDINATE c2) {return c1.xy.x < c2.xy.x; })).xy.x;
	double minY = (*min_element(begin(coordinates), end(coordinates),
		         [](swmmCOORDINATE c1, swmmCOORDINATE c2) {return c1.xy.y < c2.xy.y; })).xy.y;
	double maxX = (*max_element(begin(coordinates), end(coordinates),
		         [](swmmCOORDINATE c1, swmmCOORDINATE c2) {return c1.xy.x < c2.xy.x; })).xy.x;
	double maxY = (*max_element(begin(coordinates), end(coordinates),
		         [](swmmCOORDINATE c1, swmmCOORDINATE c2) {return c1.xy.y < c2.xy.y; })).xy.y;
		
	file << setw(15) << "DIMENSIONS" << setw(10) << minX - 0.250 << setw(10) << minY - 0.250; 
	file << setw(10) << maxX + 0.250 << setw(10) << maxY + 0.250 << endl;
	file << setw(15) << "Units" << setw(10) << "None" << endl; 
	file << endl;
}


void GrmsToSwmm::OutVertices(FileOutputStream &file)
{
	file << "[VERTICES]" << endl;
	file << ";;Link           X-Coord            Y-Coord  " << endl;
	file << ";;-------------- ------------------ ------------------" << endl;
	file << endl;
}


void GrmsToSwmm::OutProfiles(FileOutputStream &file)
{
	file << "[PROFILES]" << endl;
	file << endl;
}



//*******************************************************************************