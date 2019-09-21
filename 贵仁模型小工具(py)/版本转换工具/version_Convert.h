#pragma once
#pragma warning(disable : 4996)

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<iomanip>
#include<string>

#include<Poco/Path.h>
#include<Poco/File.h>
#include<Poco/FileStream.h>

#include"DataStreamReader.h"

using namespace std;
using namespace Poco;


struct node
{
	double x, y;
};
struct cross
{
	string name;
	int nodeNum;
	node *node = nullptr;
};
struct reach
{
	int  hdID;
	int crossNum;
	string *crossID = nullptr;
};
struct Time
{
	int xlname;
	node *time = nullptr;
};

void get_num(string file, int &crossnum, int &reachnum);
void convert_HD(string infile, string outfile);
void convert_BC(string infile1, string infile2, string outfile2);