<string> to <char*>

// 方法一
std::string str = "string";
const char *cstr = str.c_str();   //string.c_str()返回的是const char*。

// 方法二
std::string str = "string";
char* chr = const_cast<char*>(str.c_str()) // 强制去掉const限制。

// 方法三
std::string str = "string";
char *cstr = new char[str.length() + 1];   // 先确定char []的大小。
strcpy(cstr, str.c_str());        // 复制。
delete [] cstr;

// 方法四
std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);



<char*> to <string>

// 方法一
std::string str(buffer, buffer + length);  // 初始化时，使用char*。

// 方法二
str.assign(buffer, buffer + length);       // 如果string已经存在。

// 方法三
string str = buffer；                      // 直接赋值。




