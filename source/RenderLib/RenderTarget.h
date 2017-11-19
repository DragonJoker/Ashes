/**
*\file
*	RenderTarget.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderTarget_HPP___
#define ___RenderLib_RenderTarget_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

namespace render
{
	/**
	*\brief
	*	Classe de cible de rendu.
	*\remarks
	*	Une cible de rendu dessine une scène dans son frame buffer, permettant
	*	d'ajouter des effets de post-traitement avant le rendu final.
	*/
	class RenderTarget
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] dimensions
		*	Les dimensions de la texture recevant le rendu.
		*\param[in] format
		*	Le format des pixels de la texture recevant le rendu.
		*/
		RenderTarget( gl::IVec2 const & dimensions
			, gl::PixelFormat format );
		/**
		*\brief
		*	Destructeur.
		*/
		~RenderTarget()noexcept;
		/**
		*\brief
		*	Dessine la scène dans le frame buffer de la cible de rendu.
		*/
		void drawScene( Scene const & scene )const noexcept;
		/**
		*\return
		*	Les dimensions de la cible.
		*/
		inline gl::IVec2 const & dimensions()const noexcept
		{
			return m_size;
		}
		/**
		*\return
		*	La texture de couleurs.
		*/
		inline gl::Texture const & texture()const noexcept
		{
			assert( m_colour != nullptr );
			return *m_colour;
		}

	private:
		//! Les dimensions de la cible.
		gl::IVec2 m_size;
		//! Le frame buffer de la cible.
		gl::FrameBufferPtr m_framebuffer;
		//! La texture recevant le rendu couleurs.
		gl::TexturePtr m_colour;
		//! Le tampon de rendu recevant la profondeur.
		gl::RenderBufferPtr m_depth;
	};
}

#endif
