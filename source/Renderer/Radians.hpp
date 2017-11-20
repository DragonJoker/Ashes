/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Utils.hpp"

namespace utils
{
	/**
	*\brief
	*	Classe d'angle exprimé en radians.
	*\remarks
	*	Permet de renforcer les unités des angles à la compilation.\n
	*	Se construit à partir de valeurs en radians, et retourne des valeurs en
	*	radians.
	*/
	template< typename T >
	class RadiansT
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		explicit inline RadiansT( float value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline RadiansT( double value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline RadiansT( long double value )noexcept
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
		inline RadiansT & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		inline RadiansT & operator+=( RadiansT< T > const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		inline RadiansT & operator-=( RadiansT< T > const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		inline RadiansT & operator*=( T rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		inline RadiansT & operator/=( T rhs )noexcept
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
	inline bool operator==( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return std::abs( T( lhs ) - T( rhs ) ) < std::numeric_limits< T >::epsilon();
	}

	template< typename T >
	inline bool operator!=( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return T( lhs ) > T( rhs );
	}

	template< typename T >
	inline bool operator<( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return T( lhs ) < T( rhs );
	}

	template< typename T >
	inline bool operator>=( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline RadiansT< T > operator+( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp += rhs;
	}

	template< typename T >
	inline RadiansT< T > operator-( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp -= rhs;
	}

	template< typename T >
	inline RadiansT< T > operator*( RadiansT< T > const & lhs, T rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp *= rhs;
	}

	template< typename T >
	inline RadiansT< T > operator/( RadiansT< T > const & lhs, T rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
	using Radians = RadiansT< float >;
}
/**
*\brief
*	Définition de litérale pour les angles en radians.
*\param[in] value
*	La valeur en radians.
*\return
*	L'angle.
*/
inline utils::Radians operator "" _radians( long double value )
{
	return utils::Radians{ float( value ) };
}
