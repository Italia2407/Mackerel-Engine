#pragma once
#include <vector>
#include <map>
#include "MemoryChunk.h"

namespace MCK::MemoryManagement
{
	class BlockAllocator
	{
	public:
		~BlockAllocator();

		size_t standardBlockSize = 128;
		unsigned int blocksPerChunk = 12;

		void* Allocate(size_t bytes);
		void Deallocate(void* ptr);

		inline size_t AlignSize(size_t size)
		{
			return (((size-1) / standardBlockSize) + 1) * standardBlockSize;
		}

	private:
		std::vector<MemoryChunk> chunks;
		
		/**
		 * Creates a new chunk, from which memory can be allocated.
		 * Do not hold onto the chunk ptr, it may be moved
		 * \param blockSize
		 * \return A reference to the chunk.
		 */
		MemoryChunk* CreateChunk(size_t blockSize);

		/**
		 * Evaluates whether a chunk should be destroyed, destroys it if so.
		 * 
		 * \param chunkIndex
		 */
		void EvaluateChunk(unsigned int chunkIndex);
	};
}


