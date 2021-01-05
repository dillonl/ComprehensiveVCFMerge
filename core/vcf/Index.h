#pragma once

#include "utils/Types.h"
#include "utils/gzstream.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>

namespace cvm
{
	class Index
	{
	public:
		typedef std::shared_ptr< Index > SharedPtr;
		Index();
		~Index();

		static std::shared_ptr< Index > GenerateIndex(const std::string& vcfPath)
		{
			std::shared_ptr< Index > indexPtr = std::make_shared< Index >();

            std::shared_ptr< std::istream > fileStreamPtr;
			if (vcfPath.substr(vcfPath.find_last_of(".") + 1) == "gz")
			{
				auto igzstreamPtr = std::make_shared< igzstream >();
				igzstreamPtr->open(vcfPath.c_str());
				fileStreamPtr = igzstreamPtr;
			}
			else
			{
				fileStreamPtr = std::make_shared< std::ifstream >(vcfPath, std::fstream::in);
			}

			std::string line;
			/* std::cout << "starting" << std::endl; */
			fileStreamPtr->seekg(0, fileStreamPtr->end);
			auto filePos = fileStreamPtr->tellg();
			std::cout << "end file pos " << filePos << std::endl;
			fileStreamPtr->seekg(0, fileStreamPtr->beg);
			std::cout << "start file pos " << filePos << std::endl;
			std::unordered_map< std::string, long > chromsAndPositions;
			while (std::getline(*fileStreamPtr, line))
			{
				if (line.size() > 1)
				{
					if (line.c_str()[0] == '#') { continue; }
					std::string chrom = line.substr(0, line.find('\t'));
					if (chromsAndPositions.find(chrom) == chromsAndPositions.end())
					{
						std::cout << "adding chrom: " << chrom << fileStreamPtr->tellg() << std::endl;
						chromsAndPositions.emplace(chrom, fileStreamPtr->tellg());
					}
				}
			}
			for (auto iter : chromsAndPositions)
			{
				std::cout << "Chrom: " << iter.first << " " << iter.second << std::endl;
			}

			return indexPtr;
		}
	private:

	};

}
