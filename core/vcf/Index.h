#pragma once

#include "utils/Types.h"
#include "utils/gzstream.h"

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

			return indexPtr;
		}
	private:

	};

}
