/**
*\file
*	Range.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Range_HPP___
#define ___DesktopUtils_Range_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <math.h>
#include <limits>

namespace render
{
	/**
	*\brief
	*	Classe d'intervalle de valeurs.
	*/
	template< typename T >
	class Range
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] lower, upper
		*	Les bornes de l'intervalle.
		*/
		inline Range( T const & lower, T const & upper )noexcept
			: m_lower{ std::min( lower, upper ) }
			, m_upper{ std::max( lower, upper ) }
		{
			assert( m_lower != m_upper );
		}
		/**
		*\brief
		*	Remet une valeur dans l'intervalle.
		*\param[in] value
		*	La valeur.
		*\return
		*	\p m_lower si la \p value y est inférieure.\n
		*	\p m_upper si la \p value y est supérieure.\n
		*	\p value sinon.
		*/
		T const & clamp( T const & value )const noexcept
		{
			if ( value < m_lower )
			{
				return m_lower;
			}

			if ( value > m_upper )
			{
				return m_upper;
			}

			return value;
		}
		/**
		*\brief
		*	Retourne le pourcentage correspondant à la valeur donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	0.0 si \p value est égal à ou inférieur à \p m_lower.\n
		*	1.0 si \p value est égal à ou supérieur à \p m_upper.\n
		*	Une pourcentage allant de 0.0 à 1.0, selon que la valeur est plus
		*	proche de \p m_lower ou de \p m_upper.
		*/
		float percent( T const & value )const noexcept
		{
			return float( m_lower - clamp( value ) ) / float( m_lower - m_upper );
		}
		/**
		*\brief
		*	Retourne le pourcentage correspondant à la valeur donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	0.0 si \p value est égal à ou supérieur à \p m_upper.\n
		*	1.0 si \p value est égal à ou inférieur à \p m_lower.\n
		*	Une pourcentage allant de 0.0 à 1.0, selon que la valeur est plus
		*	proche de \p m_upper ou de \p m_lower.
		*/
		float invpercent( T const & value )const noexcept
		{
			return 1.0f - percent( value );
		}
		/**
		*\brief
		*	Retourne la valeur correspondant au pourcentage donné.
		*\param[in] percent
		*	Le pourcentage.
		*\return
		*	\p m_lower si \p percent vaut 0.0.\n
		*	\p m_upper si \p percent vaut 1.0.\n
		*	Une valeur comprise entre \p m_lower et \p m_upper.
		*/
		T value( float const & percent )const noexcept
		{
			return T{ m_lower + percent * float( m_upper - m_lower ) };
		}
		/**
		*\return
		*	La borne inférieure.
		*/
		inline T const & lower()const noexcept
		{
			return m_lower;
		}
		/**
		*\return
		*	La borne supérieure.
		*/
		inline T const & upper()const noexcept
		{
			return m_upper;
		}

	private:
		//! La borne inférieure.
		T m_lower;
		//! La borne supérieure.
		T m_upper;
	};
	/**
	*\brief
	*	Fonction d'aide à la construction d'un intervalle.
	*\param[in] lower, upper
	*	Les bornes de l'intervalle.
	*\return
	*	L'intervalle créé.
	*/
	template< typename T >
	inline Range< T > makeRange( T const & lower, T const & upper )noexcept
	{
		return Range< T >( lower, upper );
	}
}

#endif
