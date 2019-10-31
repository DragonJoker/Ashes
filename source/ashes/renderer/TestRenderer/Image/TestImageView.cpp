#include "Image/TestImageView.hpp"

#include "Core/TestDevice.hpp"
#include "Image/TestImage.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	ImageView::ImageView( VkDevice device
		, VkImageViewCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}
}
