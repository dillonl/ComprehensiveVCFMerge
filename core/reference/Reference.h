#pragma once

#include "utils/Types.h"
#include "utils/Noncopyable.hpp"

#include "Fasta.h"

#include <memory>
#include <future>
#include <string>
#include <unordered_map>

namespace cvm
{
	class Reference : public Noncopyable
	{
	public:
		typedef std::shared_ptr< Reference > SharedPtr;
        Reference(const std::string& filePath);
		~Reference();

		void loadReferencesAsync(const std::vector< std::string >& regions);

        char* getSequenceStringFromRegion(const std::string& region, position startPosition);

	private:
        void setReferenceIDAndSequence(const std::string& computedRegionString);
		std::shared_ptr< ::FastaReference > m_fasta_reference;

		std::unordered_map< std::string, char* > m_reference_sequences_map;
		std::unordered_map< std::string, std::future< void > > m_reference_future_map;

	};
}
