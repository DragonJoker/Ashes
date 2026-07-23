/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "util/UtilsPrerequisites.hpp"

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
		/**\{*/
		explicit DegreesT( float value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit DegreesT( double value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit DegreesT( long double value )noexcept
			: m_value{ T( value ) }
		{
		}
		/**\}*/
		/**
		*\name Conversion
		*/
		/**\{*/
		explicit operator T()const noexcept
		{
			return m_value;
		}
		/**\}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**\{*/
		DegreesT & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		DegreesT & operator+=( DegreesT< T > const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		DegreesT & operator-=( DegreesT< T > const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		DegreesT & operator*=( T rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		DegreesT & operator/=( T rhs )noexcept
		{
			m_value /= rhs;
			return *this;
		}
		/**\}*/

	private:
		T m_value;

	private:
		/**
		*\name Opérateurs de comparaison.
		*/
		/**\{*/
		friend bool operator==( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			return std::abs( T( lhs ) - T( rhs ) ) < std::numeric_limits< T >::epsilon();
		}

		friend bool operator!=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			return !( lhs == rhs );
		}

		friend bool operator>( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			return T( lhs ) > T( rhs );
		}

		friend bool operator<( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			return T( lhs ) < T( rhs );
		}

		friend bool operator>=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			return !( lhs < rhs );
		}

		friend bool operator<=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			return !( lhs < rhs );
		}
		/**\}*/
		/**
		*\name Opérateurs arithmétiques.
		*/
		/**\{*/
		friend DegreesT< T > operator+( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			DegreesT< T > tmp{ lhs };
			tmp += rhs;
			return tmp;
		}

		friend DegreesT< T > operator-( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
		{
			DegreesT< T > tmp{ lhs };
			tmp -= rhs;
			return tmp;
		}

		friend DegreesT< T > operator*( DegreesT< T > const & lhs, T rhs )noexcept
		{
			DegreesT< T > tmp{ lhs };
			tmp *= rhs;
			return tmp;
		}

		friend DegreesT< T > operator/( DegreesT< T > const & lhs, T rhs )noexcept
		{
			DegreesT< T > tmp{ lhs };
			tmp /= rhs;
			return tmp;
		}
		/**\}*/
	};
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
