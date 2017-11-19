/**
*\file
*	ExponentialRange.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_ExponentialRange_HPP___
#define ___DesktopUtils_ExponentialRange_HPP___
#pragma once

#include "Logarithm.h"

#include <math.h>
#include <limits>

namespace render
{
	/**
	*\brief
	*	Classe d'intervalle exponentiel de valeurs.
	*\remarks
	*	Implémente l'équation y = a * exp( b * x );
	*/
	template< uint32_t Base, typename T >
	class ExponentialRange
	{
	private:
		double doComputeB()const
		{
			assert( m_lower > 0 );
			assert( m_upper > m_lower );
			return log< Base >( m_upper / m_lower ) / ( m_upper - m_lower );
		}

		double doComputeA()const
		{
			return m_upper / exp< Base >( m_upper * m_b );
		}

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] lower, upper
		*	Les bornes de l'intervalle.
		*/
		inline ExponentialRange( T const & lower, T const & upper )noexcept
			: m_lower{ std::min( lower, upper ) }
			, m_upper{ std::max( lower, upper ) }
			, m_b{ doComputeB() }
			, m_a{ doComputeA() }
		{
		}
		/**
		*\brief
		*	Retourne la valeur correspondant au pourcentage donné.
		*\param[in] value
		*	Le pourcentage.
		*\return
		*	\p m_min si \p value vaut 0.0.\n
		*	\p m_max si \p value vaut 1.0.\n
		*	Une valeur comprise entre \p m_min et \p m_max.
		*/
		T value( T const & value )const noexcept
		{
			return T( m_a * exp< Base >( m_b * value ) );
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
		//! La borne inférieure de l'intervalle.
		T const m_lower;
		//! La borne supérieure de l'intervalle.
		T const m_upper;
		//! La valeur b de l'équation.
		double const m_b;
		//! La valeur a de l'équation.
		double const m_a;
	};
	/**
	*\brief
	*	Fonction d'aide à la construction d'un intervalle.
	*\param[in] lower, upper
	*	Les bornes de l'intervalle.
	*\return
	*	L'intervalle créé.
	*/
	template< uint32_t Base, typename T >
	inline ExponentialRange< Base, T > makeExponentialRange( T const & lower
		, T const & upper )noexcept
	{
		return ExponentialRange< Base, T >( lower, upper );
	}
}

#endif
