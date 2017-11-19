/**
*\file
*	Logarithm.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Logarithm_HPP___
#define ___DesktopUtils_Logarithm_HPP___
#pragma once

#include "Range.h"

#include <math.h>
#include <limits>

namespace render
{
	/**
	*\brief
	*	Retourne le logarithme dans la base voulue de la valeur donnée.
	*\arg Base
	*	La base du logarithme.
	*\param[in] value
	*	La valeur dont on veut le logarithme.
	*\return
	*	Le logarithme de la valeur, dans la base voulue.
	*/
	template< uint32_t Base, typename T >
	inline T log( T const value )noexcept
	{
		return T( ::log( value ) / ::log( Base ) );
	}
	/**
	*\brief
	*	Retourne l'exposant dans la base voulue de la valeur donnée.
	*\arg Base
	*	La base du logarithme.
	*\param[in] value
	*	La valeur dont on veut le logarithme.
	*\return
	*	L'exposant de la valeur, dans la base voulue.
	*/
	template< uint32_t Base, typename T >
	inline T exp( T const value )noexcept
	{
		return T( ::exp( value * ::log( Base ) ) );
	}
}

#endif
