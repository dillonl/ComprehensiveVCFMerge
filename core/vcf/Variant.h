#pragma once

#include "utils/Types.h"
#include "utils/Noncopyable.hpp"
#include "VCF.h"
#include "reference/Reference.h"

#include <string>
#include <memory>
#include <iostream>

namespace cvm
{
	class Variant : public Noncopyable
	{
	public:
        typedef std::shared_ptr< Variant > SharedPtr;
		Variant(VCF::SharedPtr vcfPtr, const std::vector< std::string >& variantComponents);
		~Variant();

		void parseLine();
		position getPosition() const { return m_position; }
		position getVariantSize() const { return m_variant_size; }

		static bool Comparator(const Variant::SharedPtr& a, const Variant::SharedPtr& b);
		static std::vector< Variant::SharedPtr > GetVariants(VCF::SharedPtr vcfPtr, const std::string& line);
		static void ProcessVariants(const Reference::SharedPtr referencePtr, const std::vector< Variant::SharedPtr >& variantPtrs, uint32_t flankSize);
	private:
        static void ProcessCluster(const Reference::SharedPtr referencePtr, const std::vector< Variant::SharedPtr >& variantPtrs);
		position m_position;
		std::vector< std::string > m_variant_components;
		uint32_t m_variant_size;
		VCF::SharedPtr m_vcf_ptr;
	};
}
