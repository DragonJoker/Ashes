/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <math.h>

#include "util/UtilsPrerequisites.hpp"
#include "util/Degrees.hpp"
#include "util/Radians.hpp"

namespace utils
{
	//!Constante pi.
	static constexpr long double Pi = 3.1415926535897932384626433832795028841968;
	//!Constante pi * 2.
	static constexpr long double PiMult2 = Pi * 2;
	//!Constante pi / 2.
	static constexpr long double PiDiv2 = Pi / 2;
	//!Constante de conversion de radian vers degré.
	static constexpr long double RadianToDegree = 57.295779513082320876798154814105;
	//!Constante de conversion de degré vers radian.
	static constexpr long double DegreeToRadian = 1 / RadianToDegree;
	/**
	*\name Conversions.
	*/
	/**\{*/
	/**
	*\brief
	*	Calcule la valeur en radians de l'angle en degrés donné.
	*\param[in] value
	*	L'angle en degrés.
	*\return
	*	L'angle en radians.
	*/
	template< typename T >
	inline RadiansT< T > toRadians( DegreesT< T > const & value )noexcept
	{
		return RadiansT< T >{ T( value ) * DegreeToRadian };
	}
	/**
	*\brief
	*	Calcule la valeur en degrés de l'angle en radians donné.
	*\param[in] value
	*	L'angle en radians.
	*\return
	*	L'angle en degrés.
	*/
	template< typename T >
	inline DegreesT< T > toDegrees( RadiansT< T > const & value )noexcept
	{
		return DegreesT< T >{ T( value ) * RadianToDegree };
	}
	/**\}*/
	/**
	*\brief
	*	Classe d'angle.
	*\remark
	*	Utilisée pour gérer les angles sans avoir à se préoccuper des
	*	conversions degré / radian.
	*	Sa valeur sera toujours comprise entre 0 et Pi.
	*/
	template< typename T >
	class AngleT
	{
	public:
		/**
		*\name Construction
		*/
		/**\{*/
		/**
		*\brief
		*	Constructeur, à partir d'un angle en degrés.
		*\param[in] value
		*	L'angle, exprimé en degrés.
		*/
		explicit AngleT( DegreesT< T > const & value )noexcept
			: m_radians{ toRadians( value ) }
		{
			doNormalise();
		}
		/**
		*\brief
		*	Constructeur, à partir d'un angle en radians.
		*\param[in] value
		*	L'angle, exprimé en radians.
		*/
		AngleT( RadiansT< T > const & value )noexcept
			: m_radians{ value }
		{
			doNormalise();
		}
		/**
		*\brief
		*	Constructeur par défaut, l'angle vaut 0 radians.
		*/
		AngleT()noexcept
			: m_radians{ T{ 0.0 } }
		{
		}
		/**\}*/
		/**
		*\name Conversion
		*/
		/**\{*/
		/**
		*\return
		*	La valeur de l'angle, en degrés.
		*/
		explicit operator DegreesT< T >()const noexcept
		{
			return to_degrees( m_radians );
		}
		/**
		*\return
		*	La valeur de l'angle, en radians.
		*/
		operator RadiansT< T >()const noexcept
		{
			return m_radians;
		}
		/**
		*\return
		*	La valeur de l'angle, en radians.
		*/
		explicit operator T()const noexcept
		{
			return T( m_radians );
		}
		/**\}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**\{*/
		AngleT & operator+=( AngleT< T > const & rhs )noexcept
		{
			m_radians += rhs;
			return *this;
		}

		AngleT & operator-=( AngleT< T > const & rhs )noexcept
		{
			m_radians -= rhs;
			return *this;
		}

		AngleT & operator+=( RadiansT< T > const & rhs )noexcept
		{
			m_radians += rhs;
			return *this;
		}

		AngleT & operator-=( RadiansT< T > const & rhs )noexcept
		{
			m_radians -= rhs;
			return *this;
		}

		AngleT & operator*=( T rhs )noexcept
		{
			m_radians *= rhs;
			return *this;
		}

		AngleT & operator/=( T rhs )noexcept
		{
			m_radians /= rhs;
			return *this;
		}
		/**\}*/

	private:
		/**
		*\brief
		*	Remet les valeurs de l'angle entre 0 et PI * 2.
		*/
		void doNormalise()noexcept
		{
			static RadiansT< T > zero{ T { 0 } };
			m_radians = RadiansT< T >{ T( T( m_radians ) - PiMult2 * floor( T( m_radians ) / PiMult2 ) ) };

			if ( T( m_radians ) >= PiMult2 )
			{
				m_radians = zero;
			}
			else if ( m_radians < zero )
			{
				if ( PiMult2 + T( m_radians ) == PiMult2 )
				{
					m_radians = zero;
				}
				else
				{
					m_radians = RadiansT< T >{ T( PiMult2 + T( m_radians ) ) };
				}
			}
		}

	private:
		//! La valeur de l'angle.
		RadiansT< T > m_radians;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**\{*/
	template< typename T >
	inline bool operator==( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return RadiansT< T >( lhs ) == RadiansT< T >( rhs );
	}

	template< typename T >
	inline bool operator!=( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator<( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return RadiansT< T >( lhs ) < RadiansT< T >( rhs );
	}

	template< typename T >
	inline bool operator>=( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator>( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return RadiansT< T >( lhs ) > RadiansT< T >( rhs );
	}

	template< typename T >
	inline bool operator<=( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return !( lhs > rhs );
	}
	/**\}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**\{*/
	template< typename T >
	inline AngleT< T > operator+( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp += rhs;
	}

	template< typename T >
	inline AngleT< T > operator-( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp -= rhs;
	}

	template< typename T >
	inline AngleT< T > operator*( AngleT< T > const & lhs, float rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp *= rhs;
	}

	template< typename T >
	inline AngleT< T > operator/( AngleT< T > const & lhs, float rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp /= rhs;
	}
	/**\}*/
	/**
	*\name Surcharges des fonctions trigonométriques de base.
	*/
	/**\{*/
	template< typename T >
	inline T cos( RadiansT< T > const & angle )noexcept
	{
		return ::cos( T( angle ) );
	}

	template< typename T >
	inline T sin( RadiansT< T > const & angle )noexcept
	{
		return ::sin( T( angle ) );
	}

	template< typename T >
	inline T tan( RadiansT< T > const & angle )noexcept
	{
		return ::tan( T( angle ) );
	}

	template< typename T >
	inline T cosh( RadiansT< T > const & angle )noexcept
	{
		return ::cosh( T( angle ) );
	}

	template< typename T >
	inline T sinh( RadiansT< T > const & angle )noexcept
	{
		return ::sinh( T( angle ) );
	}

	template< typename T >
	inline T tanh( RadiansT< T > const & angle )noexcept
	{
		return ::tanh( T( angle ) );
	}

	template< typename T >
	inline RadiansT< T > acos( T value )noexcept
	{
		return RadiansT< T >{ ::acos( value ) };
	}

	template< typename T >
	inline RadiansT< T > asin( T value )noexcept
	{
		return RadiansT< T >{ ::asin( value ) };
	}

	template< typename T >
	inline RadiansT< T > atan( T value )noexcept
	{
		return RadiansT< T >{ ::atan( value) };
	}
	/**\}*/
	using Angle = AngleT< float >;
}
