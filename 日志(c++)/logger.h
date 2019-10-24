



class Logger
	/*---------------------------------------------------------
	** - Porpuse:
	**   溃坝模型的日志类(采用线程安全的单例模式实现)。
	**
	** - Method:
	**   1.getLogger(),   返回全局唯一的日志指针,统一接口。
	**   2.setPath(),     设置日志文件的路径，统一接口。
	**   3.setLevel(),    设置日志文件记录等级，统一接口。
	**   4.returnPath(),  返回当前日志文件的路径。
	**   5.returnLevel(), 返回当前日志的记录等级。
	**   6.getTime(),     获取当前本地时间。
	**   7.writeLog(),    写入日志信息，模版数据类型默认是int。
	**
	** - Notice:
	**   writeLog()参数若有数组，只能是内建数据类型，自定义不行。
	---------------------------------------------------------*/
{
public:
	static Logger * getLogger();
	static bool setPath(const string& path="");
	static bool setLevel(LogLevel level=INFO);
	string returnPath();
	LogLevel returnLevel();
	const string getTime();
	
	template<typename T = int>
	bool writeLog(LogLevel level, const string& msg, 
		const vector<T>& params=vector<T>{})
	{
		if (level < s_level) return false;

		ofstream file(s_path, ios::app);
		if (!file.is_open()) return false;

		string datetime = getTime();
		file << "\n\ngrms -- " << datetime << msg << "\n";

		typename vector<T>::const_iterator it = params.begin();
		for (int n = 1; it != params.end(); n++, it++)
		{
			file << std::setw(20) << std::setiosflags(ios::left) << *it;
			if (n % 5 == 0) file << "\n";
		}
		
		file.close();
		return true;
	}

private:
	Logger() {}
	~Logger() {}
	Logger(const Logger&) {}
	Logger& operator = (const Logger&) {}
	//------------------
	static string s_path;                // 日志文件的地址。
	static LogLevel s_level;             // 日志等级。
	static Logger* s_logger;             // 日志指针，全局唯一。

private:
	class Cleaner          
	{
	public:
		~Cleaner() { delete s_logger; s_logger = nullptr; }
	};
	//------------------ 
	static Cleaner s_cleaner;
};







//=================
// Logger 
//=============================================================================

string Logger::s_path = "";
LogLevel Logger::s_level = INFO;
Logger::Cleaner Logger::s_cleaner;
Logger* Logger::s_logger = new Logger();


Logger* Logger::getLogger()
// Purpose: 获取日志指针,若日志文件存在则清空。
// Input : none.
// Output: none.
// Return: 日志指针。
{
	if (s_path == "")
		if (setPath() == 0)	
			throw std::runtime_error("Logger can't get path.\n");
	ofstream file(s_path, ios::out); 
	file << "***  GRMS  ---  DamBreakModel_v1.0  ***\n";
	file.close();

	return s_logger;
}


bool Logger::setPath(const string& path)
// Purpose: 设置日志文件路径（目录）。
// Input : 日志文件路径.
// Output: none.
// Return: 输入路径有效或采用当前执行路径成功，返回true；否则返回false。
{
	if (_access(path.c_str(), 0) == -1)  
	{
		char * buffer = nullptr;
		buffer = getcwd(nullptr, 0);     

		if (buffer)
		{
			s_path = buffer;
			s_path += "\\Logger.txt";
			return true;
		}
		else
			return false;
	}
	else
	{
		ofstream file(s_path, ios::out);
		if (file.is_open())  
		{
			s_path = path;
			file.close();
		}
		else
		{
			s_path = path + "\\Logger.txt";
		}
		
		return true;
	}
}


bool Logger::setLevel(LogLevel level)
// Purpose: 设置日志等级。
// Input : 日志等级.
// Output: none.
// Return: 如果日志等级设置成功，返回true；否则设为INFO,返回false。
{
	if (level< INFO || level > ERROR)
	{
		s_level = INFO;
		return false;
	}
	else
	{
		s_level = level;
		return true;
	}
}


string Logger::returnPath()
// Purpose: 返回日志文件路径。
// Input : none.
// Output: none.
// Return: 日志文件路径。
{
	return s_path;
}


LogLevel Logger::returnLevel()
// Purpose: 返回日志记录等级。
// Input : none.
// Output: none.
// Return: 日志等级。
{
	return s_level;
}


const string Logger::getTime()
// Purpose: 获取当前日期时间(写入日志)。
// Input : none.
// Output: none.
// Return: 当前日期时间。
{
	string datetime;
	time_t t= time(0);
	char tmp[28] = {""};

	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S  ", localtime(&t));
	datetime.assign(tmp, tmp + sizeof(tmp));

	return datetime;
}

//=============================================================================



//=================
// global vars
//=============================================================================

Logger* g_log = Logger::getLogger();  // 全局日志指针，用于本文件写日志。

//=============================================================================





