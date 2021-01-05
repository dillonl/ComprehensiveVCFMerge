#pragma once

#include "utils/Noncopyable.hpp"
//#include "Variant.h"

#include "htslib/bgzf.h"
#include "htslib/tbx.h"
#include "htslib/kseq.h"

#include <memory>
#include <string>
#include <vector>
#include <mutex>

namespace cvm
{
	class Variant;

	class VCF : public Noncopyable, public std::enable_shared_from_this< VCF >
	{
	public:
		typedef std::shared_ptr< VCF > SharedPtr;
		VCF(const std::string& path, const std::string& label);
		~VCF();

		std::vector< std::shared_ptr< Variant > > getVCFVariantsInRegion(const std::string& region);
		std::vector< std::string > getRegions();
		std::string getLabel();
		void closeVCF();

	private:
		void setRegions();

		tbx_t* m_tabix_ptr;
		htsFile* m_hts_file_ptr;

		std::mutex m_vcf_mutex;
		std::vector< std::string > m_regions;

		std::string m_path;
		std::string m_label;
	};
}
