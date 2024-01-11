/*
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_ArrayView_H___
#define ___Ashes_common_ArrayView_H___

#include <iterator>
#include <type_traits>

#pragma warning( push )
#pragma warning( disable: 4365 )
#include <vector>
#pragma warning( pop )

namespace ashes
{
	template< class IterT >
	struct IteratorTraits : std::iterator_traits< IterT >
	{
		using iterator_category = typename std::iterator_traits< IterT >::iterator_category;
		static_assert( std::is_convertible_v< iterator_category, std::random_access_iterator_tag > );
	};

	template< class TypeT >
	struct IteratorTraits< TypeT * > : std::iterator_traits< TypeT * >
	{
		using value_type = TypeT;
	};
	/**
	*\brief
	*	Templated class that provides std::vector style buffer view.
	*/
	template< typename ValueT, typename IteratorTraitsT = IteratorTraits< ValueT * > >
	class ArrayView
	{
		using my_traits = IteratorTraitsT;

	public:
		using value_type = typename my_traits::value_type;
		using reference = typename my_traits::reference;
		using pointer = value_type *;
		using const_pointer = value_type const *;
		using iterator = typename my_traits::pointer;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator< iterator >;
		using const_reverse_iterator = std::reverse_iterator< const_iterator >;

	public:
		ArrayView()noexcept
			: m_begin{ nullptr }
			, m_end{ nullptr }
		{
		}
		
		ArrayView( iterator begin, iterator end )noexcept
			: m_begin{ begin }
			, m_end{ end }
		{
		}

		reference operator[]( size_t index )noexcept
		{
			return *( m_begin + index );
		}

		reference operator[]( size_t index )const noexcept
		{
			return *( m_begin + index );
		}

		bool empty()const noexcept
		{
			return m_end == m_begin;
		}

		size_t size()const noexcept
		{
			return size_t( m_end - m_begin );
		}

		pointer data()noexcept
		{
			return &( *m_begin );
		}

		const_pointer data()const noexcept
		{
			return &( *m_begin );
		}

		reference front()noexcept
		{
			return *m_begin;
		}

		reference front()const noexcept
		{
			return *m_begin;
		}

		iterator begin()noexcept
		{
			return m_begin;
		}

		reverse_iterator rbegin()noexcept
		{
			return reverse_iterator{ end() };
		}

		iterator begin()const noexcept
		{
			return m_begin;
		}

		const_reverse_iterator rbegin()const noexcept
		{
			return reverse_iterator{ end() };
		}

		iterator cbegin()const noexcept
		{
			return m_begin;
		}

		const_reverse_iterator crbegin()const noexcept
		{
			return reverse_iterator{ cend() };
		}

		iterator end()noexcept
		{
			return m_end;
		}

		reverse_iterator rend()noexcept
		{
			return reverse_iterator{ begin() };
		}

		iterator end()const noexcept
		{
			return m_end;
		}

		const_reverse_iterator rend()const noexcept
		{
			return reverse_iterator{ begin() };
		}

		iterator cend()const noexcept
		{
			return m_end;
		}

		const_reverse_iterator crend()const noexcept
		{
			return reverse_iterator{ cbegin() };
		}

	private:
		iterator m_begin;
		iterator m_end;
	};

	template< typename IterT >
	bool operator==( ArrayView< IterT > const & lhs
		, ArrayView< IterT > const & rhs )
	{
		auto result = lhs.size() == rhs.size();
		auto itLhs = lhs.begin();
		auto itRhs = rhs.begin();

		while ( result && itLhs != lhs.end() )
		{
			result = ( *itLhs == *itRhs );
			++itLhs;
			++itRhs;
		}

		return result;
	}

	template< typename IterT >
	bool operator!=( ArrayView< IterT > const & lhs
		, ArrayView< IterT > const & rhs )
	{
		auto result = lhs.size() != rhs.size();
		auto itLhs = lhs.begin();
		auto itRhs = rhs.begin();

		while ( !result && itLhs != lhs.end() )
		{
			result = ( *itLhs != *itRhs );
			++itLhs;
			++itRhs;
		}

		return result;
	}

	template< typename IterT, typename ValueT = typename IteratorTraits< IterT >::value_type >
	ArrayView< ValueT > makeArrayView( IterT begin, IterT end )
	{
		return ArrayView< ValueT >{ &( *begin )
			, & ( *begin ) + std::distance( begin, end ) };
	}

	template< typename ValueT >
	ArrayView< ValueT > makeArrayView( ValueT * begin, ValueT * end )
	{
		return ArrayView< ValueT >{ begin, end };
	}

	template< typename IterT >
	auto makeArrayView( IterT begin, uint32_t size )
	{
		return makeArrayView( begin, begin + size );
	}

	template< typename IterT >
	auto makeArrayView( IterT begin, uint64_t size )
	{
		return makeArrayView( begin, begin + size );
	}

	template< typename ValueT, size_t N >
	auto makeArrayView( ValueT ( & buffer )[N] )
	{
		return makeArrayView( buffer, buffer + N );
	}
}

#endif
