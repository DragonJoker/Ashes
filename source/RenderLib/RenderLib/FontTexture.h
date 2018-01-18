/**
*\file
*	FontTexture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_FontTexture_H___
#define ___RenderLib_FontTexture_H___
#pragma once

#include "Font.h"
#include "Texture.h"

namespace render
{
	/**
	*\brief
	*	Contient les informations de la police et la texture associée.
	*/
	class FontTexture
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] font
		*	La police décrivant les caractères.
		*/
		FontTexture( renderer::Device const & device
			, renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, FontPtr && font );
		/**
		*\brief
		*	Constructeur.
		*\param[in] texture
		*	La texture contenant les caractères.
		*\param[in] font
		*	La police décrivant les caractères.
		*/
		FontTexture( renderer::Device const & device
			, TexturePtr texture
			, FontPtr && font );
		/**
		 *\brief
		 *	Destructeur.
		 */
		~FontTexture();
		/**
		 *\brief
		 *	Récupère la position de la glyphe voulue.
		 *\param[in] c
		 *	L'indice de la glyphe.
		 *\return
		 *	La position.
		 */
		renderer::IVec2 const & glyphPosition( char c )const;
		/**
		*\return
		*	La texture.
		*/
		inline Texture const & texture()const
		{
			return *m_texture;
		}
		/**
		*\return
		*	La texture.
		*/
		inline TexturePtr texturePtr()const
		{
			return m_texture;
		}
		/**
		*\return
		*	La description de la police.
		*/
		inline Font const & font()const
		{
			return *m_font;
		}

	private:
		using GlyphPositionMap = std::map< char, renderer::IVec2 >;

		//! La texture qui recevra les glyphes.
		TexturePtr m_texture;
		//! La desscription des glyphes.
		FontPtr m_font;
		//! Position des glyphes.
		GlyphPositionMap m_glyphsPositions;
	};
}

#endif
