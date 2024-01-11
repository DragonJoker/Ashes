/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_FlagCombination_HPP___
#define ___Ashes_common_FlagCombination_HPP___
#pragma once

#include <type_traits>

namespace ashes
{
	/**
	*\brief
	*	Template iterator class on a binary combination of flags.
	*\param FlagType
	*	The scoped enum type.
	*/
	template< typename FlagType >
	struct FlagIterator
	{
	public:
		//!@~english	The basic integer integer.
		using BaseType = typename std::underlying_type_t< FlagType >;

	public:
		/**
		* Begin ctor.
		*/
		FlagIterator( BaseType value )
			: m_initialValue{ value }
		{
			doGetNextValue();
		}
		/**
		* End ctor.
		*/
		FlagIterator()
			: m_initialValue{ 0u }
			, m_index{ sizeof( BaseType ) * 8 }
		{
		}

		FlagIterator & operator++()
		{
			doGetNextValue();
			return *this;
		}

		FlagIterator operator++( int )
		{
			FlagIterator result{ *this };
			++( *this );
			return result;
		}

		FlagType operator*()const
		{
			return m_value;
		}

		bool operator==( FlagIterator< FlagType > const & rhs )
		{
			return m_index == rhs.m_index
				&& m_initialValue == rhs.m_initialValue;
		}

		bool operator!=( FlagIterator< FlagType > const & rhs )
		{
			return m_index != rhs.m_index
				|| m_initialValue != rhs.m_initialValue;
		}

	private:
		void doGetNextValue()
		{
			auto v = BaseType{ 1u };

			do
			{
				m_value = FlagType( m_initialValue & ( v << m_index ) );
				++m_index;
			}
			while ( m_value == FlagType( 0 ) && m_index < sizeof( BaseType ) * 8 );
		}

	private:
		BaseType m_initialValue;
		size_t m_index{ 0u };
		FlagType m_value;
	};
	/**
	*\brief
	*	Template class providing implicit conversion from a scoped enum to a
	*	basic integer type.
	*\remarks
	*	Allows definition of flags, through binary operations (&, |),
	*	for which operands will be of same binary size.
	*\param FlagType
	*	The scoped enum type.
	*/
	template< typename FlagType >
	class FlagCombination
	{
	public:
		//!@~english	The basic integer integer.
		using BaseType = typename std::underlying_type_t< FlagType >;
		/**
		*\name
		*	Construction.
		*/
		/**@{*/
		constexpr FlagCombination( FlagType value )noexcept
			: m_value{ BaseType( value ) }
		{
			static_assert( sizeof( FlagType ) == sizeof( BaseType )
				, "Can't combine different size parameters" );
		}

		constexpr FlagCombination( BaseType value = BaseType{} )noexcept
			: m_value{ value }
		{
			static_assert( sizeof( FlagType ) == sizeof( BaseType )
				, "Can't combine different size parameters" );
		}
		/**\}*/
		/**
		*\name
		*	Casting.
		*/
		/**\{*/
		operator BaseType const &()const noexcept
		{
			return m_value;
		}

		BaseType value()const noexcept
		{
			return m_value;
		}
		/**
		*\name
		*	Iteration.
		*/
		/**\{*/
		FlagIterator< FlagType > begin()const noexcept
		{
			return FlagIterator< FlagType >( m_value );
		}

		FlagIterator< FlagType > end()const noexcept
		{
			return FlagIterator< FlagType >();
		}
		/**\}*/
		/**
		*\name
		*	Member binary operators.
		*/
		/**\{*/
		FlagCombination & operator&=( BaseType rhs )noexcept
		{
			m_value &= rhs;
			return *this;
		}

		FlagCombination & operator|=( BaseType rhs )noexcept
		{
			m_value |= rhs;
			return *this;
		}

		FlagCombination & operator&=( FlagType rhs )noexcept
		{
			m_value &= BaseType( rhs );
			return *this;
		}

		FlagCombination & operator|=( FlagType rhs )noexcept
		{
			m_value |= BaseType( rhs );
			return *this;
		}

		FlagCombination & operator&=( FlagCombination< FlagType > const & rhs )noexcept
		{
			m_value &= BaseType( rhs );
			return *this;
		}

		FlagCombination & operator|=( FlagCombination< FlagType > const & rhs )noexcept
		{
			m_value |= BaseType( rhs );
			return *this;
		}
		/**\}*/

	private:
		BaseType m_value;
	};
	/**
	*\name
	*	Comparison operators.
	*/
	/**\{*/
	template< typename FlagType >
	inline bool operator==( FlagCombination< FlagType > const & lhs
		, FlagCombination< FlagType > const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) == Type( rhs );
	}

	template< typename FlagType >
	inline bool operator==( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) == Type( rhs );
	}
	/**\}*/
	/**
	*\name
	*	Binary operators.
	*/
	/**\{*/
	template< typename FlagType >
	inline FlagCombination< FlagType > operator&
		( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		FlagCombination< FlagType > result{ lhs };
		return result &= rhs;
	}

	template< typename FlagType >
	inline FlagCombination< FlagType > operator|
		( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		FlagCombination< FlagType > result{ lhs };
		return result |= rhs;
	}

	template< typename FlagType >
	inline FlagCombination< FlagType > operator&
		( FlagCombination< FlagType > const & lhs
		, typename FlagCombination< FlagType >::BaseType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret &= rhs;
	}

	template< typename FlagType >
	inline FlagCombination< FlagType > operator|
		( FlagCombination< FlagType > const & lhs
		, typename FlagCombination< FlagType >::BaseType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret |= rhs;
	}
	/**\}*/
	/**
	*\brief
	*	Checks if the given flag is part of the given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag.
	*\return
	*	\p true if \p value contains \p flag.
	*/
	template< typename T, typename U >
	inline bool checkFlag( T const & value, U const & flag )noexcept
	{
		static_assert( sizeof( T ) == sizeof( U )
			, "Can't check flags for different size parameters" );
		return U( value & T( flag ) ) == flag;
	}
	/**
	*\brief
	*	Checks if the given flag is part of the given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag.
	*\return
	*	\p true if \p value contains \p flag.
	*/
	template< typename FlagType >
	inline bool checkFlag( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( value & flag ) == Type( flag );
	}
	/**
	*\brief
	*	Checks if the given flag is part of the given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag.
	*\return
	*	\p true if \p value contains \p flag.
	*/
	template< typename FlagType, typename Type >
	inline bool checkFlag( FlagCombination< FlagType > const & value
		, Type const & flag )noexcept
	{
		static_assert( sizeof( FlagType ) == sizeof( Type )
			, "Can't check flags for different size parameters" );
		return Type( value & flag ) == flag;
	}
	/**
	*\brief
	*	Adds a flag to given combination.
	*\param[in,out] value
	*	The combination.
	*\param[in] flag
	*	The flag to add.
	*\return
	*	The combination.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > & addFlag
		( FlagCombination< FlagType > & value
		, FlagType const & flag )noexcept
	{
		value |= flag;
		return value;
	}
	/**
	*\brief
	*	Adds a flag combination to the given value.
	*\param[in,out] value
	*	The value.
	*\param[in] flag
	*	The flag combination to add.
	*\return
	*	The value.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > & addFlags
		( FlagCombination< FlagType > & value
		, FlagCombination< FlagType > const & flags )noexcept
	{
		value |= flags;
		return value;
	}
	/**
	*\brief
	*	Removes a flag from given combination.
	*\param[in,out] value
	*	The combination.
	*\param[in] flag
	*	The flag to remove.
	*\return
	*	The combination.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > & remFlag
		( FlagCombination< FlagType > & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		value &= ~Type( flag );
		return value;
	}
	/**
	*\brief
	*	Adds a flag to given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag to add.
	*\return
	*	The new combination.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > addFlag
		( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		return ( value | flag );
	}
	/**
	*\brief
	*	Removes a flag from given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag to remove.
	*\return
	*	The new combination.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > remFlag
		( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return value & ~Type( flag );
	}
}
/**
*\brief
*	Implements binary combination operators on a scoped enum.
*/
#define Ashes_ImplementFlag( FlagType )\
	using FlagType##s = ashes::FlagCombination< FlagType >;\
	inline FlagType##s operator|( FlagType lhs, FlagType rhs )\
	{\
		return FlagType##s( lhs ) | rhs;\
	}\
	inline FlagType##s operator|( FlagType##s::BaseType lhs, FlagType rhs )\
	{\
		return FlagType##s( lhs ) | rhs;\
	}\
	inline FlagType##s operator|( FlagType lhs, FlagType##s::BaseType rhs )\
	{\
		return FlagType##s( lhs ) | rhs;\
	}

#endif
