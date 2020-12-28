#include <iostream>
#include <memory>

#include "utils/Params.h"

using namespace cvm;

int main(int argc, char** argv)
{
	auto paramsPtr = std::make_shared< Params >(argc, argv);
	if (paramsPtr->showHelp())
	{
		std::cout << paramsPtr->getHelpMessage() << std::endl;
		return 0;
	}
	auto usageErrors = paramsPtr->getUsageErrors();
	if (usageErrors.size() > 0)
	{
		std::cout << "Usage cvm [OPTIONS]" << std::endl;
		for (auto usageError : usageErrors)
		{
			std::cout << usageError << std::endl;
		}
		std::cout << "Try 'cvm --help' for mor information." << std::endl;
		return 1;
	}



	return 0;
}
