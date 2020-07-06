/*
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_BuddyAllocator_HPP___
#define ___Ashes_common_BuddyAllocator_HPP___

#include <cstdint>
#include <list>
#include <vector>

namespace ashes
{
	template< typename TraitsT >
	class BuddyAllocatorT
		: public TraitsT
	{
	private:
		using PointerType = typename TraitsT::PointerType;
		using Block = typename TraitsT::Block;

	public:
		/**
		*\param[in] numLevels
		*	The allocator maximum tree size.
		*\param[in] minBlockSize
		*	The minimum size for a block.
		*/
		inline BuddyAllocatorT( uint32_t numLevels
			, uint32_t minBlockSize );
		inline ~BuddyAllocatorT();
		/**
		*\param[in] size
		*	The requested memory size.
		*\return
		*	\p true if there is enough remaining memory for given size.
		*/
		inline bool hasAvailable( size_t size )const;
		/**
		 *\param[in] size
		 *	The requested memory size.
		 *\return
		 *	The memory chunk.
		 */
		inline PointerType allocate( size_t size );
		/**
		 *\param[in] pointer
		 *	The memory chunk.
		 */
		inline void deallocate( PointerType pointer );

	private:
		inline uint32_t doGetLevel( size_t size )const;
		inline size_t doGetLevelSize( uint32_t level )const;
		inline Block doAllocate( uint32_t order );
		inline void doMergeLevel( Block const & block
			, uint32_t index
			, uint32_t level );

	private:
		using FreeList = std::list< Block >;
		using PointerLevel = std::pair< size_t, uint32_t >;

	private:
		uint32_t m_numLevels;
		uint32_t m_minBlockSize;
		std::vector< FreeList > m_freeLists;
		std::vector< PointerLevel > m_allocated;
	};
}

#include "BuddyAllocator.inl"

#endif
