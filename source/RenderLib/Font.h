/**
*\file
*	FontTexture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Font_H___
#define ___RenderLib_Font_H___
#pragma once

#include "Glyph.h"

#include <algorithm>

namespace render
{
	//! Un tableau de glyphes.
	using GlyphArray = std::vector< Glyph >;
	/**
	*\brief
	*	Représentation d'une police : face, précision, et autres.
	*/
	class Font
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de la police.
		*\param[in] height
		*	La hauteur des caractères de la police.
		*/
		Font( std::string const & name, uint32_t height );
		/**
		*\brief
		*	Destructeur
		*/
		~Font();
		/**
		*\brief
		*	Ajoute la glyphe voulue.
		*\param[in] glyph
		*	La glyphe.
		*/
		void addGlyph( Glyph const & glyph );
		/**
		*\brief
		*	Dit si la police a déjà chargé la glyphe voulue.
		*\param[in] c
		*	Le caractère.
		*\return
		*	\p false si elle n'est pas chargée.
		*/
		inline bool hasGlyphAt( char c )const
		{
			return m_glyphs.end() != std::find_if( m_glyphs.begin()
				, m_glyphs.end()
				, [c]( Glyph const & glyph )
				{
					return glyph.character() == c;
				} );
		}
		/**
		*\brief
		*	Récupère la glyphe du caractère voulu.
		*\param[in] c
		*	Le caractère voulu.
		*\return
		*	Une référence constante sur la glyphe.
		*/
		inline Glyph const & at( char c )const
		{
			auto it = std::find_if( m_glyphs.begin()
				, m_glyphs.end()
				, [c]( Glyph const & glyph )
				{
					return glyph.character() == c;
				} );

			if ( it == m_glyphs.end() )
			{
				assert( false && "Font subscript out of range" );
			}

			return *it;
		}
		/**
		*\brief
		*	Récupère la glyphe du caractère voulu.
		*\param[in] c
		*	Le caractère voulu.
		*\return
		*	Une référence sur la glyphe.
		*/
		inline Glyph & at( char c )
		{
			auto it = std::find_if( m_glyphs.begin()
				, m_glyphs.end()
				, [c]( Glyph const & glyph )
				{
					return glyph.character() == c;
				} );

			if ( it == m_glyphs.end() )
			{
				assert( false && "Font subscript out of range" );
			}

			return *it;
		}
		/**
		*\brief
		*	Récupère la glyphe du caractère voulu.
		*\param[in] c
		*	Le caractère voulu.
		*\return
		*	Une référence sur la glyphe.
		*/
		inline Glyph const & operator[]( char c )const
		{
			assert( hasGlyphAt( c ) );

			return *std::find_if( m_glyphs.begin()
				, m_glyphs.end()
				, [c]( Glyph const & glyph )
				{
					return glyph.character() == c;
				} );
		}
		/**
		*\brief
		*	Récupère la glyphe du caractère voulu.
		*\param[in] c
		*	Le caractère voulu.
		*\return
		*	Une référence sur la glyphe.
		*/
		inline Glyph & operator[]( char c )
		{
			assert( hasGlyphAt( c ) );

			return *std::find_if( m_glyphs.begin()
				, m_glyphs.end()
				, [c]( Glyph const & glyph )
				{
					return glyph.character() == c;
				} );
		}
		/**
		*\return
		*	La hauteur de la police.
		*/
		inline uint32_t height()const
		{
			return m_height;
		}
		/**
		*\return
		*	La hauteur maximale des glyphes.
		*/
		inline uint32_t maxHeight()const
		{
			return m_maxHeight;
		}
		/**
		*\return
		*	La largeur maximale des glyphes.
		*/
		inline uint32_t maxWidth()const
		{
			return m_maxWidth;
		}
		/**
		*\return
		*	Le noùm de la police.
		*/
		inline std::string const & faceName()const
		{
			return m_faceName;
		}
		/**
		*\return
		*	Un itérateur sur le début des glyphes.
		*/
		inline auto begin()
		{
			return m_glyphs.begin();
		}
		/**
		*\return
		*	Un itérateur sur le début des glyphes.
		*/
		inline auto begin()const
		{
			return m_glyphs.begin();
		}
		/**
		*\return
		*	Un itérateur sur la fin des glyphes.
		*/
		inline auto end()
		{
			return m_glyphs.end();
		}
		/**
		*\return
		*	Un itérateur sur la fin des glyphes.
		*/
		inline auto end()const
		{
			return m_glyphs.end();
		}

	private:
		//! La hauteur de la police.
		uint32_t m_height;
		//! Le tableau de glyphes chargées.
		GlyphArray m_glyphs;
		//! Le nom de la police.
		std::string m_faceName;
		//! La hauteur maximale des glyphes.
		int m_maxHeight{ 0 };
		//! La largeur maximale des glyphes.
		int m_maxWidth{ 0 };
	};
}

#endif
