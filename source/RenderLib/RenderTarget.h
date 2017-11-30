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

#include <Renderer/RenderPass.hpp>

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
		RenderTarget( renderer::Device const & device
			, utils::IVec2 const & dimensions
			, utils::PixelFormat format );
		/**
		*\brief
		*	Destructeur.
		*/
		~RenderTarget()noexcept;
		/**
		*\brief
		*	Met à jour les données de la scène en VRAM.
		*/
		void updateScene( renderer::CommandBuffer const & commandBuffer
			, Scene & scene )const;
		/**
		*\brief
		*	Effectue les commandes pré-rendu.
		*/
		void preRenderCommands( renderer::CommandBuffer const & commandBuffer )const;
		/**
		*\brief
		*	Dessine la scène dans le frame buffer de la cible de rendu.
		*/
		void render( renderer::CommandBuffer const & commandBuffer
			, Scene const & scene )const;
		/**
		*\brief
		*	Effectue les commandes post-rendu.
		*/
		void postRenderCommands( renderer::CommandBuffer const & commandBuffer )const;
		/**
		*\return
		*	Les dimensions de la cible.
		*/
		inline utils::IVec2 const & getDimensions()const noexcept
		{
			return m_size;
		}
		/**
		*\return
		*	La texture de couleurs.
		*/
		inline renderer::Texture const & getTexture()const noexcept
		{
			assert( m_colour != nullptr );
			return *m_colour;
		}
		/**
		*\return
		*	La texture de couleurs.
		*/
		inline renderer::RenderPass const & getRenderPass()const noexcept
		{
			assert( m_renderPass != nullptr );
			return *m_renderPass;
		}
		/**
		*\return
		*	Le tampon d'images.
		*/
		inline renderer::FrameBuffer const & getFrameBuffer()const noexcept
		{
			assert( m_framebuffer != nullptr );
			return *m_framebuffer;
		}

	private:
		//! Les dimensions de la cible.
		utils::IVec2 m_size;
		//! La passe de rendu.
		renderer::RenderPassPtr m_renderPass;
		//! La texture recevant le rendu couleurs.
		renderer::TexturePtr m_colour;
		//! Le tampon de rendu recevant la profondeur.
		renderer::TexturePtr m_depth;
		//! Le frame buffer de la cible.
		renderer::FrameBufferPtr m_framebuffer;
		//! Le tampon de transfert.
		renderer::StagingBufferPtr m_stagingBuffer;
	};
}

#endif
