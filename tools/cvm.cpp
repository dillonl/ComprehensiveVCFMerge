#include <iostream>
#include <memory>
#include <thread>
#include <queue>
#include <tuple>
#include <unordered_map>

#include "utils/Params.h"
#include "utils/Utility.h"
#include "utils/ThreadPool.hpp"
#include "vcf/Variant.h"
#include "vcf/VCF.h"
#include "reference/Reference.h"

using namespace cvm;

void setVCFPtrsAndRegions(const std::vector< std::string >& vcfPaths, const std::vector< std::string >& vcfLabels, std::vector< VCF::SharedPtr >& vcfPtrs, std::vector< std::string >& regions)
{
	for (auto i = 0; i < vcfPaths.size(); ++i)
	{
		auto vcfPtr = std::make_shared< VCF >(vcfPaths[i], vcfLabels[i]);
		auto tmpRegions = vcfPtr->getRegions();
		regions.insert(regions.end(), tmpRegions.begin(), tmpRegions.end());
		vcfPtrs.emplace_back(vcfPtr);
	}
	std::sort(regions.begin(), regions.end(), [] (std::string a, std::string b) {
			if (a.rfind("chr", 0) == 0) { a = a.substr(3); }
			if (b.rfind("chr", 0) == 0) { b = b.substr(3); }
			if (isNumber(a) && isNumber(b)) { return stoi(a) < stoi(b); }
			else if (isNumber(a)) { return true; }
			else if (isNumber(b)) { return false; }
			else { return a.compare(b) < 0; }
		});
	regions.erase(std::unique(regions.begin(), regions.end()), regions.end());
}

std::vector< Variant::SharedPtr > processRegion(const std::vector< VCF::SharedPtr >& vcfPtrs, const std::string& region)
{
	std::vector< Variant::SharedPtr > variantPtrs;
	std::mutex vcfLock;
	std::queue< std::future< std::vector< Variant::SharedPtr > > > futures;
	for (auto vcfPtr : vcfPtrs)
	{
		futures.push(ThreadPool::Instance()->enqueue([&vcfLock, &variantPtrs, vcfPtr, region] () { return vcfPtr->getVCFVariantsInRegion(region); }));
	}
	while (!futures.empty())
	{
		futures.front().wait();
		auto tmpVariantPtrs = futures.front().get();
		variantPtrs.insert(variantPtrs.begin(), tmpVariantPtrs.begin(), tmpVariantPtrs.end());
		futures.pop();
	}
	std::sort(variantPtrs.begin(), variantPtrs.end(), Variant::Comparator); // uses custom Variant comparator

	uint32_t flankSize = 1000;
	Variant::ProcessVariants(nullptr, variantPtrs, flankSize);

	return variantPtrs;
}

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
	Reference::SharedPtr referencePtr = std::make_shared< Reference >(paramsPtr->getReferencePath());
	std::vector< std::string > vcfPaths = paramsPtr->getInVCFPaths();
	std::vector< std::string > vcfLabels = paramsPtr->getInVCFLabels();
	std::vector< VCF::SharedPtr > vcfPtrs;
	std::vector< std::string > regions;
	setVCFPtrsAndRegions(vcfPaths, vcfLabels, vcfPtrs, regions);
	referencePtr->loadReferencesAsync(regions);

	std::queue< std::future< std::tuple< std::string, std::vector< Variant::SharedPtr > > > > regionFutures;
	for (auto region : regions)
	{
		auto regionFunct = [&vcfPtrs, &vcfLabels, region] ()
			{
				return std::make_tuple(region, processRegion(vcfPtrs, region));
			};
		regionFutures.push(ThreadPool::Instance()->enqueue(regionFunct));
	}
	std::unordered_map< std::string, std::vector< Variant::SharedPtr > > regionVariantPtrsMap;
	while (!regionFutures.empty())
	{
		regionFutures.front().wait();
		auto result = regionFutures.front().get();
		regionFutures.pop();
		regionVariantPtrsMap.emplace(std::get< 0 >(result), std::move(std::get< 1 >(result)));
	}
	return 0;
}
