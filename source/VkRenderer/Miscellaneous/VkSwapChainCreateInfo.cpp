#include "VkRendererPrerequisites.hpp"

#include "Core/VkSurface.hpp"
#include "Core/VkSwapChain.hpp"

namespace vk_renderer
{
	VkSwapchainCreateInfoKHR convert( ashes::SwapChainCreateInfo const & value )
	{
		return VkSwapchainCreateInfoKHR
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			convert( value.flags ),
			static_cast< Surface const & >( value.surface.get() ),
			value.minImageCount,
			convert( value.imageFormat ),
			convert( value.imageColorSpace ),
			convert( value.imageExtent ),
			value.imageArrayLayers,
			convert( value.imageUsage ),
			convert( value.imageSharingMode ),
			uint32_t( value.queueFamilyIndices.size() ),
			value.queueFamilyIndices.data(),
			convert( value.preTransform ),
			convert( value.compositeAlpha ),
			convert( value.presentMode ),
			VkBool32( value.clipped
				? VK_TRUE
				: VK_FALSE ),
			( bool( value.oldSwapchain )
				? VkSwapchainKHR( static_cast< SwapChain const & >( value.oldSwapchain.value().get() ) )
				: VK_NULL_HANDLE )
		};
	}
}
