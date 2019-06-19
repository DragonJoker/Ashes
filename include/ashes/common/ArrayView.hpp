/*
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_ArrayView_H___
#define ___Ashes_common_ArrayView_H___

#include <iterator>

namespace ashes
{
	template< class IterT >
	using IteratorCategoryT = typename std::iterator_traits< IterT >::iterator_category;

	template< class IterT >
	using IteratorReferenceT = typename std::iterator_traits< IterT >::reference;

	template< class IterT >
	using IteratorPointerT = typename std::iterator_traits< IterT >::pointer;

	template< class TypeT, class = void >
	constexpr bool IsIteratorV = false;

	template< class TypeT >
	constexpr bool IsIteratorV< TypeT, std::void_t< IteratorCategoryT< TypeT > > > = true;

	template< class TypeT, class = void >
	constexpr bool IsPointerIteratorV = false;

	template< class TypeT >
	constexpr bool IsPointerIteratorV< TypeT, std::enable_if_t< IsIteratorV< TypeT > && std::is_same_v< TypeT, IteratorPointerT< TypeT > > > > = true;

	template< typename TypeT >
	struct RemovePtr
	{
		using value_type = TypeT;
	};

	template< typename TypeT >
	struct RemovePtr< TypeT * >
	{
		using value_type = TypeT;
	};

	template< typename TypeT >
	using RemovePtrT = typename RemovePtr< TypeT >::value_type;

	template< typename IterT
		, bool Enable = IsPointerIteratorV< IterT > >
	struct IteratorValueIterator
	{
		using type = RemovePtrT< IterT >;
	};

	template< typename IterT >
	struct IteratorValueIterator< IterT, false >
	{
		using type = typename std::iterator_traits< IterT >::value_type;
	};

	template< typename IterT
		, bool Enable = IsIteratorV< IterT > >
	struct IteratorValue
		: IteratorValueIterator< IterT >
	{
		using type = typename IteratorValueIterator< IterT >::type;
	};

	template< typename IterT
		, bool Enable = std::is_object_v< IterT > >
	struct IteratorValueBase
		: std::iterator_traits< IterT * >
	{
		using type = IterT *;
	};

	template< typename IterT >
	struct IteratorValueBase< IterT, false >
	{
		// Empty for non iterable stuff.
	};

	template< typename IterT >
	struct IteratorValue< IterT, false >
		: IteratorValueBase< IterT >
	{
		using type = typename IteratorValueBase< IterT >::type;
	};

	template< typename IterT >
	using IteratorValueT = typename IteratorValue< IterT >::type;

	template< typename IterT
		, bool Enable = IsPointerIteratorV< IterT > >
	struct IteratorArrayIterator
	{
		template< typename IterT1, typename IterT2 >
		static void checkRange( IterT1 begin, IterT1 end
			, IterT2 & outBegin, IterT2 & outEnd )
		{
			outBegin = begin;
			outEnd = end;

			if ( !outBegin || !outEnd )
			{
				outEnd = outBegin;
			}
		}
	};

	template< typename IterT >
	struct IteratorArrayIterator< IterT, false >
	{
		template< typename IterT1, typename IterT2 >
		static void checkRange( IterT1 begin, IterT1 end
			, IterT2 & outBegin, IterT2 & outEnd )
		{
			outBegin = &( *begin );
			outEnd = &( *end );
		}
	};

	template< typename ValueT
		, bool Enable = IsIteratorV< ValueT > >
	struct ArrayIterator
		: IteratorArrayIterator< ValueT >
	{
		using type = ValueT;
	};

	template< typename ValueT
		, bool Enable = std::is_object_v< ValueT > >
	struct BaseArrayIterator
		: std::iterator_traits< ValueT * >
	{
		using type = ValueT *;
		
		template< typename IterT1, typename IterT2 >
		static void checkRange( IterT1 begin, IterT1 end
			, IterT2 & outBegin, IterT2 & outEnd )
		{
			outBegin = begin;
			outEnd = end;

			if ( !outBegin || !outEnd )
			{
				outEnd = outBegin;
			}
		}
	};

	template< typename ValueT >
	struct BaseArrayIterator< ValueT, false >
	{
		// Empty for non iterable stuff.
	};

	template< typename ValueT >
	struct ArrayIterator< ValueT, false >
		: BaseArrayIterator< ValueT >
	{
		using type = typename BaseArrayIterator< ValueT >::type;
	};

	template< typename ValueT >
	using ArrayIteratorT = typename ArrayIterator< ValueT >::type;

	template< typename IterT >
	struct ArrayViewTypeTraits
	{
		using iterator_category = IteratorCategoryT< IterT >;
		static_assert( std::is_convertible_v< iterator_category, std::random_access_iterator_tag > );

		using value_type = IteratorValueT< IterT >;
		using reference = IteratorReferenceT< IterT >;
		using pointer = IteratorPointerT< IterT >;

		using iterator = IterT;
		using const_iterator = const iterator;

		template< typename IterT >
		static void checkRange( IterT begin, IterT end
			, iterator & outBegin, iterator & outEnd )
		{
			ArrayIterator< IterT >::checkRange( std::move( begin ), std::move( end )
				, outBegin, outEnd );
		}
	};
	/**
	*\brief
	*	Templated class that provides std::vector style buffer view.
	*/
	template< typename ValueT >
	class ArrayView
	{
		using my_traits = ArrayViewTypeTraits< ValueT * >;

	public:
		using value_type = typename my_traits::value_type;
		using reference = typename my_traits::reference;
		using pointer = typename my_traits::pointer;
		using iterator = typename my_traits::iterator;
		using const_iterator = typename my_traits::const_iterator;
		using reverse_iterator = std::reverse_iterator< iterator >;
		using const_reverse_iterator = std::reverse_iterator< const_iterator >;

	public:
		template< typename IterT >
		ArrayView( IterT begin, IterT end )noexcept
		{
			my_traits::checkRange( std::move( begin ), std::move( end )
				, m_begin, m_end );
		}

		reference operator[]( size_t index )noexcept
		{
			return m_begin + index;
		}

		const reference operator[]( size_t index )const noexcept
		{
			return m_begin + index;
		}

		bool empty()const noexcept
		{
			return m_end == m_begin;
		}

		size_t size()const noexcept
		{
			return m_end - m_begin;
		}

		pointer data()noexcept
		{
			return &( *m_begin );
		}

		pointer const data()const noexcept
		{
			return &( *m_begin );
		}

		iterator begin()noexcept
		{
			return m_begin;
		}

		reverse_iterator rbegin()noexcept
		{
			return reverse_iterator{ end() };
		}

		const_iterator begin()const noexcept
		{
			return m_begin;
		}

		const_reverse_iterator rbegin()const noexcept
		{
			return reverse_iterator{ end() };
		}

		const_iterator cbegin()const noexcept
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

		const_iterator end()const noexcept
		{
			return m_end;
		}

		const_reverse_iterator rend()const noexcept
		{
			return reverse_iterator{ begin() };
		}

		const_iterator cend()const noexcept
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

	template< typename ValueT >
	bool operator==( ArrayView< ValueT > const & lhs
		, ArrayView< ValueT > const & rhs )
	{
		auto result = lhs.size() == rhs.size();
		auto itLhs = lhs.begin();
		auto itRhs = rhs.begin();

		while ( result && itLhs != lhs.end() )
		{
			result = ( *itLhs == *itRhs );
		}

		return result;
	}

	template< typename IterT >
	ArrayView< IteratorValueT< IterT > > makeArrayView( IterT begin, IterT end )
	{
		return ArrayView< IteratorValueT< IterT > >{ begin, end };
	}

	template< typename IterT >
	ArrayView< IteratorValueT< IterT > > makeArrayView( IterT begin, uint32_t size )
	{
		return makeArrayView( begin, begin + size );
	}

	template< typename IterT >
	ArrayView< IteratorValueT< IterT > > makeArrayView( IterT begin, uint64_t size )
	{
		return makeArrayView( begin, begin + size );
	}

	template< typename ValueT, size_t N >
	ArrayView< ValueT > makeArrayView( ValueT( & buffer )[N] )
	{
		return makeArrayView( buffer, buffer + N );
	}
}

#endif
