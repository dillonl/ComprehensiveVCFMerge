#include "Params.h"
#include "Utility.hpp"

namespace cvm
{
	Params::Params(int argc, char** argv) : m_options("CVM", " - Comprehensive VCF Merger (CVM) merges multiple VCF files into a single VCF. CVM reconstructs the sequence of each VCF's alleles with the reference sequence and compares them across all VCFs to discover duplicate variants no matter how the alleles are distributed.")
	{
		parse(argc, argv);
	}

	Params::~Params()
	{
	}

	void Params::parse(int argc, char** argv)
	{
		this->m_options.add_options()
			("h,help","Print help message")
			("s,silent", "Run without reports")
			("r,reference", "Path to input reference (FASTA) file [required]", cxxopts::value< std::string >())
			("v,vcfs", "Path to input VCF files, separate multiple files by space [required]", cxxopts::value< std::vector< std::string > >())
		    ("n,names", "Name for VCF files, separate multiple names by space [required]", cxxopts::value< std::vector< std::string > >())
			("o,output_vcf", "Output VCF file name [required]", cxxopts::value< std::string >());
		this->m_options.parse(argc, argv);
	}

	std::vector< std::string > Params::getUsageErrors()
	{
		std::vector< std::string > errorMessages;
		auto vcfCount = m_options.count("v");
		auto nameCount = m_options.count("n");
		if (vcfCount <= 1 || nameCount <= 1)
		{
			errorMessages.emplace_back("You must provide at least 2 vcfs and names");
		}
		if (vcfCount != nameCount)
		{
			errorMessages.emplace_back("Please provide a name for every vcf");
		}
		return errorMessages;
	}

	bool Params::showHelp()
	{
		return m_options.count("h");
	}

	bool Params::isSilent()
	{
		return m_options.count("s");
	}

	std::string Params::getHelpMessage()
	{
		return this->m_options.help();
	}

	std::vector< std::string > Params::getInVCFPaths()
	{
		auto vcfPaths =  m_options["v"].as< std::vector< std::string > >();
		validateFilePaths(vcfPaths, true);
		return vcfPaths;
	}

	std::vector< std::string > Params::getInVCFNames()
	{
		auto vcfNames =  m_options["n"].as< std::vector< std::string > >();
		return vcfNames;
	}

	std::string Params::getOutVCFPath()
	{
		auto vcfName =  m_options["o"].as< std::string >();
		return vcfName;
	}

	void Params::validateFilePaths(const std::vector< std::string >& paths, bool exitOnFailure)
	{
		for (auto path : paths)
		{
			fileExists(path, exitOnFailure);
		}
	}
}
