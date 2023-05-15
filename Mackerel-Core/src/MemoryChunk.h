#pragma once

namespace MCK::MemoryManagement
{
	/**
	 * Describes a chunk of memory
	 * A chunk is composed of multiple, equally sized blocks.
	 */
	struct MemoryChunk
	{
		size_t blockSize = 512;
		void* chunk = nullptr;
		bool* blocksOccupied = nullptr;
		unsigned int numBlocksOccupied = 0;
		unsigned int numBlocks = 0;

		/**
		 * Allocates a block within this chunk.
		 * 
		 * \return A ptr to the block
		 */
		void* Allocate();

		/**
		 * Whether this chunk contains a ptr.
		 * 
		 * \param ptr
		 * \return True if the pointer is inside this chunk
		 */
		bool Contains(void* ptr);

		/**
		 * Deallocates a block within this chunk.
		 * 
		 * \param ptr
		 */
		void Deallocate(void* ptr);

		/**
		 * Deallocates the entire chunk.
		 * 
		 */
		void FreeChunk();
	};
}
