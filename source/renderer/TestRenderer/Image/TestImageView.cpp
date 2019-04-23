#include "Image/TestImageView.hpp"

#include "Core/TestDevice.hpp"
#include "Image/TestImage.hpp"

namespace test_renderer
{
	ImageView::ImageView( Device const & device
		, Image const & image
		, ashes::ImageViewCreateInfo const & createInfo )
		: ashes::ImageView{ device
			, image
			, createInfo }
	{
	}

	ImageView::~ImageView()
	{
	}
}
