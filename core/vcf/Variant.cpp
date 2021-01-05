#include "Variant.h"

#include "utils/Utility.h"

#include <iostream>

namespace cvm
{
	Variant::Variant(VCF::SharedPtr vcfPtr, const std::vector< std::string >& variantComponents) :
		m_vcf_ptr(vcfPtr),
		m_variant_components(variantComponents),
		m_position(stol(variantComponents[1]))
	{
		m_variant_size = variantComponents[4].size() > variantComponents[3].size() ? variantComponents[4].size() : variantComponents[3].size();
	}

	Variant::~Variant()
	{
	}

	/* static */
	bool Variant::Comparator(const Variant::SharedPtr& a, const Variant::SharedPtr& b)
	{
		if (a->getPosition() == b->getPosition())
		{
			return a->getVariantSize() < b->getVariantSize();
		}
		else
		{
			return a->getPosition() < b->getPosition();
		}
	}

	/* static */
	std::vector< Variant::SharedPtr > Variant::GetVariants(VCF::SharedPtr vcfPtr, const std::string& line)
	{
		std::vector< Variant::SharedPtr > variants;
		std::vector< std::string > variantComponents;
		split(line, '\t', variantComponents);
		if (variantComponents[4].find(",") != std::string::npos)
		{
			std::vector< std::string > alts;
			split(variantComponents[4], ',', alts);
			for (auto alt : alts)
			{
				variantComponents[4] = alt;
				variants.emplace_back(std::make_shared< Variant >(vcfPtr, variantComponents));
			}
		}
		else
		{
			variants.emplace_back(std::make_shared< Variant >(vcfPtr, variantComponents));
		}
		return variants;
	}

	/* static */
	void Variant::ProcessVariants(const Reference::SharedPtr referencePtr, const std::vector< Variant::SharedPtr >& variantPtrs, uint32_t flankSize)
	{
		std::vector< Variant::SharedPtr > variantClusterPtrs;
		for (int i = 0; i < variantPtrs.size() - 1; ++i)
		{
			if (variantClusterPtrs.size() == 0 ||
				(variantClusterPtrs[variantClusterPtrs.size() - 1]->getPosition() + flankSize) < variantPtrs[i]->getPosition())
			{
				variantClusterPtrs.emplace_back(variantPtrs[i]);
			}
			else if (variantClusterPtrs.size() > 0)
			{
				Variant::ProcessCluster(referencePtr, variantClusterPtrs);
				variantClusterPtrs.clear();
			}
		}
	}

	/* static */
	void Variant::ProcessCluster(const Reference::SharedPtr referencePtr, const std::vector< Variant::SharedPtr >& variantPtrs)
	{
		// referencePtr->getSequenceStringFromRegion(variantPtrs[0]->m_variant_components[0], variantPtrs[0]->m_position);
	}
}
