/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_TextureView_HPP___
#define ___TestRenderer_TextureView_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Image/TextureView.hpp>

namespace test_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class TextureView
		: public ashes::TextureView
	{
	public:
		TextureView( Device const & device
			, Texture const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		~TextureView();
	};
}

#endif
