#include "MemoryChunk.h"
#include <cstdlib>

namespace MCK::MemoryManagement
{
	void* MemoryChunk::Allocate()
	{
		// Search for an unoccupied block
		for (unsigned int i = 0; i < numBlocks; ++i)
		{
			if (!blocksOccupied[i])
			{
				// Mark as occupied
				blocksOccupied[i] = true;
				++numBlocksOccupied;

				// Calculate offset
				size_t offset = i * blockSize;

				// Arithematic
				char* blockPtr = (char*)chunk;
				blockPtr += offset;

				return (void*)blockPtr;
			}
		}

		return nullptr;
	}

	bool MemoryChunk::Contains(void* ptr)
	{
		char* start = (char*)chunk;
		char* end = start + numBlocks * blockSize;

		return ptr > start && ptr <= end;
	}

	void MemoryChunk::Deallocate(void* ptr)
	{
		// Search for the address
		for (unsigned int i = 0; i < numBlocks; ++i)
		{
			// Calculate offset
			size_t offset = i * blockSize;

			// Arithematic
			char* blockPtr = (char*)chunk;
			blockPtr += offset;

			// If found
			if ((void*)blockPtr == ptr && blocksOccupied[i] == true)
			{
				// Mark unoccupied
				blocksOccupied[i] = false;
				--numBlocksOccupied;
				return;
			}
			
		}
	}

	void MemoryChunk::FreeChunk()
	{
		free(chunk);
		free(blocksOccupied);

		chunk = nullptr;
		blocksOccupied = nullptr;
		numBlocks = 0;
		numBlocksOccupied = 0;
	}
}
