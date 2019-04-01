/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Texture_HPP___
#define ___GlRenderer_Texture_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Image/Image.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Image
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\remarks
		*	Used for swapchain own image.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Image( Device const & device
			, VkFormat format
			, VkExtent2D const & dimensions );
		/**
		*\brief
		*	Constructeur.
		*\remarks
		*	Used for swapchain images creation.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Image( Device const & device
			, Image const & image );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Image( Device const & device
			, VkImageCreateInfo const & createInfo );
		/**
		*\brief
		*	Destructeur.
		*/
		~Image();
		/**
		*\copydoc	ashes::Image::getMemoryRequirements
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\copydoc	ashes::Image::bindMemory
		*/
		void bindMemory( DeviceMemoryPtr memory )const;
		/**
		*\copydoc	ashes::Image::getMemoryRequirements
		*/
		void generateMipmaps( CommandBuffer & commandBuffer )const;
		/**
		*\copydoc	ashes::Image::createView
		*/
		ImageViewPtr createView( VkImageViewCreateInfo const & createInfo )const;

		inline bool hasImage()const noexcept
		{
			return m_texture != GL_INVALID_INDEX;
		}
		/**
		*\return
		*	L'image OpenGL.
		*/
		inline bool hasInternal()const noexcept
		{
			return m_texture != GL_INVALID_INDEX;
		}
		/**
		*\return
		*	L'image OpenGL.
		*/
		inline GLuint getInternal()const noexcept
		{
			assert( hasInternal() );
			return m_texture;
		}

		VkImageCreateInfo const createInfo;

	private:
		Device const & m_device;
		GlTextureType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
		bool m_ownTexture{ true };
		mutable DeviceMemoryPtr m_storage;
	};
}

#endif
