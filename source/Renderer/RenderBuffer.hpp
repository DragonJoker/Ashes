/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Texture_HPP___
#define ___Renderer_Texture_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <Utils/Vec2.hpp>

#include <VkLib/Image.hpp>
#include <VkLib/Sampler.hpp>

namespace renderer
{
	/**
	*\brief
	*	Un wrapper autour d'une texture ne pouvant être utilisée qu'en destination de rendu.
	*/
	class RenderBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		RenderBuffer( Device const & device
			, utils::PixelFormat format
			, IVec2 const & size );
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeColourAttachment()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur et stencil.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeDepthStencilAttachment()const;
		/**
		*\return
		*	Le format des pixels de la texture.
		*/
		inline utils::PixelFormat getFormat()const noexcept
		{
			return m_format;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline utils::IVec2 const & getDimensions()const noexcept
		{
			return m_size;
		}
		/**
		*\return
		*	L'image vulkan.
		*/
		inline vk::Image const & getImage()const noexcept
		{
			assert( m_texture );
			return *m_texture;
		}

	private:
		renderer::Device const & m_device;
		utils::IVec2 m_size;
		utils::PixelFormat m_format{ utils::PixelFormat::eR8G8B8 };
		vk::ImagePtr m_texture;
	};
}

#endif
