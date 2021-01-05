#pragma once

#include <string>
#include <vector>


namespace cvm
{
    void split(const std::string& s, char c, std::vector< std::string >& v);
    bool fileExists(const std::string& name, bool exitOnFailure);
    bool folderExists(const std::string& path, bool exitOnFailure);
    bool endsWith(const std::string& path, const std::string& ending);
    bool isNumber(const std::string& s);
}
