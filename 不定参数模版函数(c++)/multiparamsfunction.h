
	template<typename T>
	bool writeTestLog(const T& value)
	{
		ofstream file(s_path, ios::app);
		if (!file.is_open()) return false;
		file << value << "\n";
		file.close();	
		return true;
	}

	template<typename T, typename... Args>
	bool writeTestLog(const T& value, const Args&... args)
	{
		ofstream file(s_path, ios::app);
		if (!file.is_open()) return false;

		if (typeid(value).name() == typeid(s_level).name())
		{
			if (value < s_level) return false;

			string datetime = getTime();
			file << "\n\ntest -- " << datetime << "results:\n";
		}
		else
		{
			file <<value<<"\t";
		}
		
		file.close();

		writeTestLog(args...);
		return true;
	}



