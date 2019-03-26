/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Image/ImageView.hpp>

namespace gl_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class ImageView
		: public ashes::ImageView
	{
	public:
		ImageView( Device const & device
			, Image const & image );
		ImageView( Device const & device
			, Image const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		~ImageView();

		inline GlTextureType getTarget()const noexcept
		{
			return m_target;
		}

	private:
		Device const & m_device;
		GlTextureType m_target;
	};
}

#endif
