/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Image/ImageView.hpp>

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
	class ImageView
		: public ashes::ImageView
	{
	public:
		ImageView( Device const & device
			, Image const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		~ImageView();
	};
}
