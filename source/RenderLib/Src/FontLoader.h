/**
*\file
*	FontLoader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_FontLoader_HPP___
#define ___RenderLib_FontLoader_HPP___
#pragma once

#include "Glyph.h"

namespace render
{
	/**
	*\brief
	*	Classe de base d'un chargeur de police.
	*\brief
	*	Doit être implémentée pour les plateformes disponibles.
	*/
	class FontLoader
	{
	public:
		/**
		*\brief
		*	Initialise le loader pour la taille de police donnée.
		*\param[in] height
		*	La hauteur de police.
		*/
		virtual void initialise( uint32_t height ) = 0;
		/**
		*\brief
		*	Charge la glyphe associée au caractère donné.
		*\param[in] c
		*	Le caractère.
		*\return
		*	La glyphe chargée.
		*/
		virtual render::Glyph loadGlyph( char c )const = 0;
	};
}

#endif
