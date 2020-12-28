#pragma once

#include "Noncopyable.hpp"

#include <string>
#include <vector>
#include <cxxopts.hpp>

namespace cvm
{
	class Params : Noncopyable
	{
	public:
		Params(int argc, char** argv);
		~Params();

		std::vector< std::string > getUsageErrors();
		bool showHelp();
		bool isSilent();

		void validateFilePaths(const std::vector< std::string >& paths, bool exitOnFailure);
		std::vector< std::string > getInVCFPaths();
		std::vector< std::string > getInVCFNames();
		std::string getOutVCFPath();
		std::string getHelpMessage();

	private:
		void parse(int argc, char** argv);
		cxxopts::Options m_options;
	};
}
