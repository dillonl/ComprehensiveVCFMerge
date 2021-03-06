#include "Utility.h"

#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace cvm
{
	void split(const std::string& s, char c, std::vector< std::string >& v)
	{
		std::string::size_type i = 0;
		std::string::size_type j = s.find(c);

		while (j != std::string::npos)
		{
			v.push_back(s.substr(i, j-i));
			i = ++j;
			j = s.find(c, j);

			if (j == std::string::npos)
			{
				v.push_back(s.substr(i, s.length()));
			}
		}
	}

	bool fileExists(const std::string& name, bool exitOnFailure)
	{
	    std::ifstream f(name.c_str());
	    if (!f)
	    {
			if (exitOnFailure)
			{
				std::cout << "File not found: " << name << std::endl;
				exit(EXIT_FAILURE);
			}
			return false;
		}
		return true;
	}

	bool folderExists(const std::string& path, bool exitOnFailure)
	{
		if(!path.empty())
		{
			struct stat sb;

			if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
			{
				return true;
			}
		}
		if (exitOnFailure)
		{
			std::cout << "Folder not found: " << path << std::endl;
			exit(EXIT_FAILURE);
		}
		return false;
	}

	bool endsWith(const std::string& path, const std::string& ending)
	{
		auto iter = path.find_last_of(ending);
		return (iter != path.size() - ending.size());
	}

	bool isNumber(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}
}
