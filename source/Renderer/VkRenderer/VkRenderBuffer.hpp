/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_RenderBuffer_HPP___
#define ___VkRenderer_RenderBuffer_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include "VkTexture.hpp"
#include <Renderer/RenderBuffer.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Un wrapper autour d'une texture ne pouvant être utilisée qu'en destination de rendu.
	*/
	class RenderBuffer
		: public renderer::RenderBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		RenderBuffer( renderer::Device const & device
			, utils::PixelFormat format
			, renderer::IVec2 const & size );
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeColourAttachment()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur et stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilAttachment()const override;
		/**
		*\return
		*	L'image vulkan.
		*/
		inline vk::Image const & getImage()const noexcept
		{
			return *m_image;
		}

	private:
		vk::ImagePtr m_image;
		Texture m_texture;
	};
}

#endif
