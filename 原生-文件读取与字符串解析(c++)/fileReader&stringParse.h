#include<string>
#include<sstream>


vector<string> splitByStrtok(const char* s, const char* delim)
// 输入待拆分字符串s，分隔符delim。
{
	vector<string> ret;

	char* temp = new char[strlen(s) + 1]; // 常指针可以计算长度(自动将const char*转string)。
	strcpy(temp, s);                      // 拷贝字符串。

	char* word = strtok(temp, delim);     // 第一次需要输入待拆分字符串，返回拆分的第一个字符串。
	ret.push_back(word);
	while (word = strtok(nullptr, delim)) // 后续不需要输入字符串。
		ret.push_back(word);

	delete[] temp;
	return ret;
}

vector<string> splitByStringStream(const string& s, char delim)
// 输入待拆分字符串s，分隔符delim。
{
	std::stringstream ss(s);         // 初始化stringstream对象。
	string temp;
	vector<string> ret;

	while (getline(ss, temp, delim)) //getline(),从ss输入流取字符至字符串temp，直至遇到delim或\n。
		ret.push_back(temp);         

	return ret;
}

vector<string> splitBySTLfind(const string& s, const string& delim)
{
	size_t front = 0;                            // 子字符串首字符位置。
	size_t back = s.find_first_of(delim, front); // 子字符串末字符位置。

	vector<string> ret;

	while (back != string::npos)                 // npos表示不存在的位置。
	{
		ret.push_back(s.substr(front, back - front));  // string 获取[front, back)子字符串。
		front = back + 1;
		back = s.find_first_of(delim, front);    // 从front开始，第一个delim的位置。
	}

	if (back - front > 0)                        // 若字符串末端还有字符，依然存入。
	{
		ret.push_back(s.substr(front, back - front));
	}

	return ret;
}



vector<XYd> readFile(const char* path)
{
	std::ifstream fin(path);	      
	if (!fin.is_open()) return vector<XYd>{};  

	std::string line;
	vector<XYd> tv;
	XYd temp;

	while (getline(fin, line, '\n'))         // 按行读取。
	{
		double x, y;
		vector<string> data;
	  

		//data = splitByStrtok(line.c_str(), "\t");

		//data = splitByStringStream(line, '\t');

		data = splitBySTLfind(line, "\t");


		x = atof(data[0].c_str());     // string 转 double。
		y = atof(data[1].c_str());
		temp.x = x;
		temp.y = y;
		tv.push_back(temp);
	}

	fin.close();
	return tv;
}