/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Image/TextureView.hpp>

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
