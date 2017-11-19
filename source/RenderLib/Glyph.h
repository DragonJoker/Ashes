/**
*\file
*	Glyph.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Glyph_H___
#define ___RenderLib_Glyph_H___
#pragma once

#include "RenderLibPrerequisites.h"

namespace render
{
	/**
	*\brief
	*	Représentation d'un caractère d'une Font (glyphe).
	*\remark
	*	Contient la position, taille et autres informations.
	*/
	class Glyph
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] c
		*	Le caractère de la glyphe.
		*\param[in] size
		*	Les dimensions de la glyphe.
		*\param[in] bearing
		*	La position de la glyphe par rapport au curseur.
		*\param[in] advance
		*	Nombre de pixels pour placer le caractère suivant.
		*\param[in] bitmap
		*	Contient les données de la glyphe.
		*/
		inline Glyph( char c
			, gl::IVec2 const & size
			, gl::IVec2 const & bearing
			, uint32_t advance
			, ByteArray const & bitmap )
			: m_bearing{ bearing }
			, m_size{ size }
			, m_advance{ advance }
			, m_character{ c }
			, m_bitmap{ bitmap }
		{
		}
		/**
		*\brief
		*	Récupère le caractère de la glyphe.
		*\return
		*	La valeur.
		*/
		inline char character()const
		{
			return m_character;
		}
		/**
		*\return
		*	Les dimensions de la glyphe.
		*/
		inline gl::IVec2 const & size()const
		{
			return m_size;
		}
		/**
		*\return
		*	La position de la glyphe par rapport au curseur.
		*/
		inline gl::IVec2 const & bearing()const
		{
			return m_bearing;
		}
		/**
		*\return
		*	Le nombre de pixels pour place la prochaine glyphe.
		*/
		inline uint32_t advance()const
		{
			return m_advance;
		}
		/**
		*\return
		*	Le bitmap contenant la glyphe.
		*/
		inline ByteArray const & bitmap()const
		{
			return m_bitmap;
		}
		/**
		*\return
		*	Vide le bitmap.
		*/
		inline void clearBitmap()
		{
			m_bitmap.clear();
		}

	private:
		//! Position de la glyphe par rapport au curseur.
		gl::IVec2 const m_bearing;
		//! Dimensions de la glyphe.
		gl::IVec2 const m_size;
		//! Nombre de pixels pour placer le caractère suivant.
		uint32_t const m_advance;
		//! Caractère de la glyphe.
		char const m_character;
		//! Le bitmap contenant la glyphe.
		ByteArray m_bitmap;
	};
	/**
	*\brief
	*	Opérateur de comparaison d'égalité.
	*\param[in] lhs, rhs
	*	Les valeurs à comparer.
	*/
	inline bool operator==( Glyph const & lhs, Glyph const & rhs )
	{
		return lhs.character() == rhs.character();
	}
	/**
	*\brief
	*	Opérateur de comparaison de différence.
	*\param[in] lhs, rhs
	*	Les valeurs à comparer.
	*/
	inline bool operator!=( Glyph const & lhs, Glyph const & rhs )
	{
		return lhs.character() != rhs.character();
	}
	/**
	*\brief
	*	Opérateur de comparaison inférieur.
	*\param[in] lhs, rhs
	*	Les valeurs à comparer.
	*/
	inline bool operator<( Glyph const & lhs, Glyph const & rhs )
	{
		return lhs.character() < rhs.character();
	}
	/**
	*\brief
	*	Opérateur de comparaison supérieur.
	*\param[in] lhs, rhs
	*	Les valeurs à comparer.
	*/
	inline bool operator>( Glyph const & lhs, Glyph const & rhs )
	{
		return lhs.character() > rhs.character();
	}
	/**
	*\brief
	*	Opérateur de comparaison inférieur ou égal.
	*\param[in] lhs, rhs
	*	Les valeurs à comparer.
	*/
	inline bool operator<=( Glyph const & lhs, Glyph const & rhs )
	{
		return lhs.character() <= rhs.character();
	}
	/**
	*\brief
	*	Opérateur de comparaison supérieur ou égal.
	*\param[in] lhs, rhs
	*	Les valeurs à comparer.
	*/
	inline bool operator>=( Glyph const & lhs, Glyph const & rhs )
	{
		return lhs.character() >= rhs.character();
	}
}

#endif
