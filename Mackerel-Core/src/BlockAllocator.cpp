#include "BlockAllocator.h"

namespace MCK::MemoryManagement
{
	MemoryChunk* BlockAllocator::CreateChunk(size_t blockSize)
	{
		MemoryChunk chunkInfo;
		chunkInfo.blockSize = blockSize;
		chunkInfo.numBlocks = blocksPerChunk;
		chunkInfo.numBlocksOccupied = 0;

		chunkInfo.chunk = malloc(blockSize * blocksPerChunk);
		chunkInfo.blocksOccupied = (bool*)malloc(sizeof(bool) * blocksPerChunk);

		for (unsigned int i = 0; i < blocksPerChunk; ++i)
		{
			chunkInfo.blocksOccupied[i] = false;
		}

		unsigned int chunkIndex = unsigned int(chunks.size());
		chunks.push_back(chunkInfo);

		return &chunks[chunkIndex];
	}

	void* BlockAllocator::Allocate(size_t size)
	{
		// Search for a chunk
		for (unsigned int i = 0; i < chunks.size(); ++i)
		{
			// With sufficient block sizes and a free block
			if (chunks[i].blockSize >= size && 
				chunks[i].numBlocksOccupied < chunks[i].numBlocks)
			{
				// Allocate in this chunk
				return chunks[i].Allocate();
			}
		}

		// If no chunk found, create one
		MemoryChunk* newChunk = CreateChunk(AlignSize(size));
		return newChunk->Allocate();
	}

	void BlockAllocator::Deallocate(void* ptr)
	{
		for (unsigned int i = 0; i < chunks.size(); ++i)
		{
			if (chunks[i].Contains(ptr))
			{
				chunks[i].Deallocate(ptr);
				EvaluateChunk(i);

				break;
			}
		}
	}

	void BlockAllocator::EvaluateChunk(unsigned int chunkIndex)
	{
		if (chunks[chunkIndex].numBlocksOccupied <= 0)
		{
			chunks[chunkIndex].FreeChunk();
			chunks.erase(chunks.begin() + chunkIndex);
		}
	}

	BlockAllocator::~BlockAllocator()
	{
		for (unsigned int i = 0; i < chunks.size(); ++i)
		{
			chunks[i].FreeChunk();
		}

		chunks.clear();
	}
}
