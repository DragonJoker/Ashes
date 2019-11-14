/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "util/Vec4.hpp"

namespace utils
{
	/**
	*\brief
	*	Appelle une fonction sur chaque composante du vecteur donné.
	*\remarks
	*	Permet d'appeler, par exemple, des fonctions math�matiques telles que
	*	abs, cos... sur les composantes d'un vecteur, ce en un seul
	*	appel, et de stocker le résultat dans un vecteur.
	*\param[in] func
	*	La fonction.
	*\param[in] vector
	*	Le vecteur (utils::Vec2, utils::Vec3 ou utils::Vec4)
	*\return
	*	Le vecteur résultant des appels.
	*/
	template< typename R, typename T, typename V >
	inline V vectorCall( R( *func )( T ), V const & vector );
	/**
	*\brief
	*	Appelle une fonction sur chaque composante du vecteur donné.
	*\remarks
	*	Permet d'appeler, par exemple, des fonctions math�matiques telles que
	*	abs, cos... sur les composantes d'un vecteur, ce en un seul
	*	appel, et de stocker le résultat dans un vecteur.
	*	Version pour les fonctions prenant en paramêtre une r�f�rence constante
	*	(comme utils::cos, par exemple).
	*\param[in] func
	*	La fonction.
	*\param[in] vector
	*	Le vecteur (utils::Vec2, utils::Vec3 ou utils::Vec4)
	*\return
	*	Le vecteur résultant des appels.
	*/
	template< typename R, typename T, typename V >
	inline V vectorCall( R( *func )( T const & ), V const & vector );
}

#include "util/Vectorisation.inl"
