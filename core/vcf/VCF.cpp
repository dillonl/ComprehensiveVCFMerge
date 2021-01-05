#include "VCF.h"
#include "Variant.h"

#include "htslib/hts.h"

#include <iostream>
#include <algorithm>

namespace cvm
{
	VCF::VCF(const std::string& path, const std::string& label) :
		m_path(path),
		m_label(label)
	{
		if ((m_hts_file_ptr = hts_open(path.c_str(), "r")) == 0)
		{
			std::cerr << "vcf failed to open" << std::endl;
			exit(1);
		}
		if ((m_tabix_ptr = tbx_index_load(path.c_str())) == NULL)
		{
			std::cerr << "vcf index file failed to open" << std::endl;
			exit(1);
		}
		setRegions();
	}

	VCF::~VCF()
	{
		hts_close(m_hts_file_ptr);
		tbx_destroy(m_tabix_ptr);
	}

	void VCF::setRegions()
	{
		std::lock_guard< std::mutex > l(this->m_vcf_mutex);
		this->m_regions.clear();
		int n;
		const char** tmp = tbx_seqnames(this->m_tabix_ptr, &n);
		for (auto i = 0; i < n; ++i)
		{
			this->m_regions.emplace_back(tmp[i]);
		}
		free(tmp);
	}

	std::vector< std::string > VCF::getRegions()
	{
		std::lock_guard< std::mutex > l(this->m_vcf_mutex);
		return this->m_regions;
	}

	std::string VCF::getLabel()
	{
		return this->m_label;
	}

	void VCF::closeVCF()
	{
		hts_close(m_hts_file_ptr);
		tbx_destroy(m_tabix_ptr);
	}

	std::vector< Variant::SharedPtr > VCF::getVCFVariantsInRegion(const std::string& region)
	{
		std::lock_guard< std::mutex > l(this->m_vcf_mutex);
		std::vector< Variant::SharedPtr > variants;
		if (std::find(this->m_regions.begin(), this->m_regions.end(), region) != this->m_regions.end())
		{
			hts_itr_t* iter = tbx_itr_querys(this->m_tabix_ptr, region.c_str());
			kstring_t tmpLine;
			tmpLine.l = 0;
			tmpLine.m = 0;
			tmpLine.s = NULL;
			while (iter && tbx_itr_next(this->m_hts_file_ptr, this->m_tabix_ptr, iter, &tmpLine) >= 0)
			{
				auto lineVariants = Variant::GetVariants(shared_from_this(), std::string(tmpLine.s));
				for (auto variant : lineVariants)
				{
					variants.emplace_back(variant);
				}
			}
			tbx_itr_destroy(iter);
			free(tmpLine.s);
		}
		return variants;
	}
}
