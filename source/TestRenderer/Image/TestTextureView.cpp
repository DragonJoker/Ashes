#include "Image/TestTextureView.hpp"

#include "Core/TestDevice.hpp"
#include "Image/TestTexture.hpp"

namespace test_renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & image
		, ashes::ImageViewCreateInfo const & createInfo )
		: ashes::TextureView{ device
			, image
			, createInfo }
	{
	}

	TextureView::~TextureView()
	{
	}
}
