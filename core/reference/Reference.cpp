#include "Reference.h"

#include "utils/ThreadPool.hpp"

namespace cvm
{
	Reference::Reference(const std::string& filePath)
	{
		m_fasta_reference = std::make_shared< ::FastaReference >();
		m_fasta_reference->open(filePath);
	}

	Reference::~Reference()
	{
		// m_fasta_reference is closed when its destructor is called so we don't worry about it
	}


	void Reference::loadReferencesAsync(const std::vector< std::string >& regions)
	{
		std::mutex refLock;
		std::queue< std::future< void > > regionFutures;
		for (auto region : regions)
		{
			auto funct = [this, &region, &refLock] ()
				{
					std::cout << "loading region: " << region << std::endl;
					auto seq = this->m_fasta_reference->getSequence(region);
					std::transform(seq.begin(), seq.end(), seq.begin(), ::toupper);
					char* cSeq = new char[seq.length() + 1];
					strcpy(cSeq, seq.c_str());
					std::lock_guard< std::mutex > l(refLock);
					this->m_reference_sequences_map.emplace(region, cSeq);
				};
			this->m_reference_future_map.emplace(region, ThreadPool::Instance()->enqueue(funct));
		}
	}

	char* Reference::getSequenceStringFromRegion(const std::string& region, position startPosition)
	{
		auto futureIter = this->m_reference_future_map.find(region);
		if (futureIter != this->m_reference_future_map.end())
		{
			std::cout << "waiting for " << region << " to load" << std::endl;
			futureIter->second.wait();
			this->m_reference_future_map.erase(region);
			std::cout << region << " loaded" << std::endl;
		}
		auto iter = this->m_reference_sequences_map.find(region);
		if (iter != this->m_reference_sequences_map.end())
		{
			return iter->second + startPosition;
		}
		else
		{
			return nullptr;
		}
	}
}
