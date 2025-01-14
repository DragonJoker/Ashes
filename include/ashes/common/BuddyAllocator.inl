#include <cassert>

namespace ashes
{
	template< typename TraitsT >
	inline BuddyAllocatorT< TraitsT >::BuddyAllocatorT( uint32_t numLevels
		, uint32_t minBlockSize )
		: TraitsT{ minBlockSize * ( 1ull << numLevels ), minBlockSize }
		, m_numLevels{ numLevels }
		, m_minBlockSize{ minBlockSize }
	{
		assert( numLevels + minBlockSize < 64u );
		m_freeLists.resize( m_numLevels + 1 );
		m_freeLists[0u].push_back( Block{ this->getPointer( 0u ) } );
	}

	template< typename TraitsT >
	inline BuddyAllocatorT< TraitsT >::~BuddyAllocatorT()
	{
	}

	template< typename TraitsT >
	inline size_t BuddyAllocatorT< TraitsT >::getAvailable()const noexcept
	{
		size_t result{};

		for ( auto const & allocation : m_allocated )
		{
			result += doGetLevelSize( allocation.second );
		}

		return result;
	}

	template< typename TraitsT >
	inline bool BuddyAllocatorT< TraitsT >::hasAvailable( size_t size )const
	{
		auto level = doGetLevel( size );
		bool result = !size;

		if ( !result && size <= this->getSize() )
		{
			auto it = m_freeLists.rend() - ( level + 1 );

			while ( it != m_freeLists.rend() && it->empty() )
			{
				++it;
			}

			result = it != m_freeLists.rend();
		}

		return result;
	}

	template< typename TraitsT >
	inline typename BuddyAllocatorT< TraitsT >::PointerType BuddyAllocatorT< TraitsT >::allocate( size_t size )
	{
		typename BuddyAllocatorT< TraitsT >::PointerType result{};

		if ( size <= this->getSize() )
		{
			auto level = doGetLevel( size );
			result = doAllocate( level ).data;
			m_allocated.emplace_back( this->getOffset( result ), level );
			TraitsT::registerAllocation( result, size, doGetLevelSize( level ) );
		}

		return result;
	}

	template< typename TraitsT >
	inline void BuddyAllocatorT< TraitsT >::deallocate( typename BuddyAllocatorT< TraitsT >::PointerType pointer )
	{
		auto offset = this->getOffset( pointer );
		auto it = std::find_if( m_allocated.begin()
			, m_allocated.end()
			, [offset]( PointerLevel const & ptrLevel )
			{
				return ptrLevel.first == offset;
			} );
		assert( it != m_allocated.end() );

		if ( it != m_allocated.end() )
		{
			auto level = it->second;
			auto index = uint32_t( offset / doGetLevelSize( level ) );
			Block block{ pointer };
			TraitsT::registerDeallocation( pointer, doGetLevelSize( level ) );
			m_allocated.erase( it );
			doMergeLevel( block, index, level );
		}
	}

	template< typename TraitsT >
	inline uint32_t BuddyAllocatorT< TraitsT >::doGetLevel( size_t size )const
	{
		auto levelSize = this->getSize();
		uint32_t level = 0;

		while ( size < levelSize )
		{
			levelSize >>= 1;
			++level;
		}

		if ( size > levelSize )
		{
			--level;
		}

		return std::min( level, m_numLevels );
	}

	template< typename TraitsT >
	inline size_t BuddyAllocatorT< TraitsT >::doGetLevelSize( uint32_t level )const
	{
		return this->getSize() / size_t( 1ull << level );
	}

	template< typename TraitsT >
	inline typename BuddyAllocatorT< TraitsT >::Block BuddyAllocatorT< TraitsT >::doAllocate( uint32_t level )
	{
		auto & freeList = m_freeLists[level];

		if ( freeList.empty() )
		{
			if ( level == 0 )
			{
				return this->getNull();
			}

			auto buddy = doAllocate( level - 1 );

			if ( this->isNull( buddy.data ) )
			{
				return buddy;
			}

			freeList.push_back( Block{ PointerType( buddy.data ) } );
			freeList.push_back( Block{ PointerType( buddy.data + doGetLevelSize( level ) ) } );
		}

		auto buddy = freeList.front();
		freeList.erase( freeList.begin() );
		return buddy;
	}

	template< typename TraitsT >
	inline void BuddyAllocatorT< TraitsT >::doMergeLevel( typename BuddyAllocatorT< TraitsT >::Block const & block
		, uint32_t pindex
		, uint32_t level )
	{
		auto & freeList = m_freeLists[level];
		Block lhs{};
		Block rhs{};
		typename FreeList::iterator it;

		if ( pindex % 2u )
		{
			// RHS block => find LHS in free blocks of same level.
			lhs.data = PointerType( block.data - doGetLevelSize( level ) );
			it = std::find_if( freeList.begin()
				, freeList.end()
				, [lhs]( Block const & lookUp )
			{
				return lookUp.data == lhs.data;
			} );
			rhs = block;
		}
		else
		{
			// LHS block => find RHS in free blocks of same level.
			lhs = block;
			rhs.data = PointerType( block.data + doGetLevelSize( level ) );
			it = std::find_if( freeList.begin()
				, freeList.end()
				, [rhs]( Block const & lookUp )
			{
				return lookUp.data == rhs.data;
			} );
		}

		if ( it != freeList.end() )
		{
			// remove the other block from the list.
			freeList.erase( it );
			// add lhs to lower level list.
			auto offset = this->getOffset( lhs.data );
			auto index = uint32_t( offset / doGetLevelSize( level - 1u ) );
			// merge lower level.
			doMergeLevel( lhs, index, level - 1u );
		}
		else
		{
			freeList.push_back( block );
		}
	}
}
