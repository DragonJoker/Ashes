/**
*\file
*	RangedValue.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_RangedValue_HPP___
#define ___DesktopUtils_RangedValue_HPP___
#pragma once

#include "Range.h"

namespace render
{
	/**
	*\brief
	*	Représente une valeur dans un intervalle.
	*\remarks
	*	La valeur ne peut pas sortir de son intervalle
	*/
	template< typename T >
	class RangedValue
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] value
		*	La valeur.
		*\param[in] range
		*	Son intervalle.
		*/
		inline RangedValue( T const & value
			, Range< T > const & range )noexcept
			: m_range{ range }
			, m_value{ m_range.clamp( value ) }
		{
		}
		/**
		*\brief
		*	Affectation depuis une valeur.
		*/
		inline RangedValue & operator=( T const & value )noexcept
		{
			m_value = m_range.clamp( value );
			return *this;
		}
		/**
		*\brief
		*	Redéfinit l'intervalle.
		*\param[in] range
		*	Le nouvel intervalle.
		*/
		void updateRange( Range< T > const & range )noexcept
		{
			m_range = range;
			m_range.clamp( m_value );
		}
		/**
		*\return
		*	0.0 si \p m_value est égal à ou inférieur à \p m_value.m_min.\n
		*	1.0 si \p m_value est égal à ou supérieur à \p m_value.m_max.\n
		*	Une valeur allant de 0.0 à 1.0, selon que la valeur est plus proche
		*	de \p m_value.m_min ou de \p m_value.m_max.
		*/
		float percent()const noexcept
		{
			return m_range.percent( m_value );
		}
		/**
		*\return
		*	La valeur.
		*/
		inline T const & value()const noexcept
		{
			return m_value;
		}
		/**
		*\return
		*	L'intervalle.
		*/
		inline Range< T > const & range()const noexcept
		{
			return m_range;
		}
		/**
		*\name Opérateurs arithmétiques.
		*/
		/**@{*/
		inline RangedValue< T > & operator+=( RangedValue< T > const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value + rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator-=( RangedValue< T > const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value - rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator*=( RangedValue< T > const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value * rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator/=( RangedValue< T > const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value / rhs.m_value );
			return *this;
		}

		inline RangedValue< T > & operator+=( T const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value + rhs );
			return *this;
		}

		inline RangedValue< T > & operator-=( T const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value - rhs );
			return *this;
		}

		inline RangedValue< T > & operator*=( T const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value * rhs );
			return *this;
		}

		inline RangedValue< T > & operator/=( T const & rhs )noexcept
		{
			m_value = m_range.clamp( m_value / rhs );
			return *this;
		}
		/**@}*/

	private:
		//! L'intervalle.
		Range< T > m_range;
		//! La valeur.
		T m_value;
	};
	/**
	*\brief
	*	Fonction d'aide à la construction d'une valeur dans un intervalle.
	*\param[in] value
	*	La valeur.
	*\param[in] min, max
	*	Les bornes de l'intervalle.
	*\return
	*	L'objet créé.
	*/
	template< typename T >
	inline RangedValue< T > makeRangedValue( T const & value
		, T const & min
		, T const & max )noexcept
	{
		return RangedValue< T >{ value, makeRange( min, max ) };
	}
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( RangedValue< T > const & lhs
		, T const & rhs )noexcept
	{
		static constexpr auto eps = std::numeric_limits< T >::epsilon();
		return std::abs( lhs.value() - rhs ) < eps;
	}

	template< typename T >
	inline bool operator!=( RangedValue< T > const & lhs
		, T const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( RangedValue< T > const & lhs
		, T const & rhs )noexcept
	{
		return lhs.value() > rhs;
	}

	template< typename T >
	inline bool operator<( RangedValue< T > const & lhs
		, T const & rhs )noexcept
	{
		return lhs.value() < rhs;
	}

	template< typename T >
	inline bool operator>=( RangedValue< T > const & lhs
		, T const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( RangedValue< T > const & lhs
		, T const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator==( T const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		static constexpr auto eps = std::numeric_limits< T >::epsilon();
		return std::abs( lhs - rhs.value() ) < eps;
	}

	template< typename T >
	inline bool operator!=( T const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( T const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs > rhs.value();
	}

	template< typename T >
	inline bool operator<( T const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs < rhs.value();
	}

	template< typename T >
	inline bool operator>=( T const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( T const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator==( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() == rhs.value();
	}

	template< typename T >
	inline bool operator!=( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() > rhs.value();
	}

	template< typename T >
	inline bool operator<( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() < rhs.value();
	}

	template< typename T >
	inline bool operator>=( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline T operator+( T const & lhs, RangedValue< T > const & rhs )noexcept
	{
		return lhs + rhs.value();
	}

	template< typename T >
	inline T operator-( T const & lhs, RangedValue< T > const & rhs )noexcept
	{
		return lhs - rhs.value();
	}

	template< typename T >
	inline T operator*( T const & lhs, RangedValue< T > const & rhs )noexcept
	{
		return lhs * rhs.value();
	}

	template< typename T >
	inline T operator/( T const & lhs, RangedValue< T > const & rhs )noexcept
	{
		return lhs / rhs.value();
	}

	template< typename T >
	inline T operator+( RangedValue< T > const & lhs, T const & rhs )noexcept
	{
		return lhs.value() + rhs;
	}

	template< typename T >
	inline T operator-( RangedValue< T > const & lhs, T const & rhs )noexcept
	{
		return lhs.value() - rhs;
	}

	template< typename T >
	inline T operator*( RangedValue< T > const & lhs, T const & rhs )noexcept
	{
		return lhs.value() * rhs;
	}

	template< typename T >
	inline T operator/( RangedValue< T > const & lhs, T const & rhs )noexcept
	{
		return lhs.value() / rhs;
	}

	template< typename T >
	inline T operator+( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() + rhs.value();
	}

	template< typename T >
	inline T operator-( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() - rhs.value();
	}

	template< typename T >
	inline T operator*( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() * rhs.value();
	}

	template< typename T >
	inline T operator/( RangedValue< T > const & lhs
		, RangedValue< T > const & rhs )noexcept
	{
		return lhs.value() / rhs.value();
	}
	/**@}*/
}

#endif
