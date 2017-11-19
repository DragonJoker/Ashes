/**
*\file
*	Degrees.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Utils_Degrees_HPP___
#define ___Utils_Degrees_HPP___
#pragma once

#include "Utils.h"

namespace utils
{
	/**
	*\brief
	*	Classe d'angle exprimé en degrés.
	*\remark
	*	Permet de renforcer les unités des angles à la compilation.\n
	*	Se construit à partir de valeurs en degrés, et retourne des valeurs en
	*	degrés.
	*/
	template< typename T >
	class DegreesT
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		explicit inline DegreesT( float value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline DegreesT( double value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline DegreesT( long double value )noexcept
			: m_value{ T( value ) }
		{
		}
		/**@}*/
		/**
		*\name Conversion
		*/
		/**@{*/
		explicit inline operator T()const noexcept
		{
			return m_value;
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline DegreesT & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		inline DegreesT & operator+=( DegreesT< T > const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		inline DegreesT & operator-=( DegreesT< T > const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		inline DegreesT & operator*=( T rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		inline DegreesT & operator/=( T rhs )noexcept
		{
			m_value /= rhs;
			return *this;
		}
		/**@}*/

	private:
		T m_value;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return std::abs( T( lhs ) - T( rhs ) ) < std::numeric_limits< T >::epsilon();
	}

	template< typename T >
	inline bool operator!=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return T( lhs ) > T( rhs );
	}

	template< typename T >
	inline bool operator<( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return T( lhs ) < T( rhs );
	}

	template< typename T >
	inline bool operator>=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline DegreesT< T > operator+( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp += rhs;
	}

	template< typename T >
	inline DegreesT< T > operator-( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp -= rhs;
	}

	template< typename T >
	inline DegreesT< T > operator*( DegreesT< T > const & lhs, T rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp *= rhs;
	}

	template< typename T >
	inline DegreesT< T > operator/( DegreesT< T > const & lhs, T rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
	using Degrees = DegreesT< float >;
}
/**
*\brief
*	Définition de litérale pour les angles en degrés.
*\param[in] value
*	La valeur en degrés.
*\return
*	L'angle.
*/
inline utils::Degrees operator "" _degrees( long double value )
{
	return utils::Degrees{ float( value ) };
}

#endif
