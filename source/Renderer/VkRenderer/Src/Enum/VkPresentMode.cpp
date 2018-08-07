#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPresentModeKHR convert( renderer::PresentMode const & value )
	{
		switch ( value )
		{
		case renderer::PresentMode::eImmediate:
			return VK_PRESENT_MODE_IMMEDIATE_KHR;

		case renderer::PresentMode::eMailbox:
			return VK_PRESENT_MODE_MAILBOX_KHR;

		case renderer::PresentMode::eFifo:
			return VK_PRESENT_MODE_FIFO_KHR;

		case renderer::PresentMode::eFifoRelaxed:
			return VK_PRESENT_MODE_FIFO_RELAXED_KHR;

		case renderer::PresentMode::eSharedDemandRefresh:
			return VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;

		case renderer::PresentMode::eSharedContinuousRefresh:
			return VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;

		default:
			assert( false && "Unsupported PresentMode" );
			return VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	renderer::PresentMode convert( VkPresentModeKHR const & value )
	{
		switch ( value )
		{
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			return renderer::PresentMode::eImmediate;

		case VK_PRESENT_MODE_MAILBOX_KHR:
			return renderer::PresentMode::eMailbox;

		case VK_PRESENT_MODE_FIFO_KHR:
			return renderer::PresentMode::eFifo;

		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			return renderer::PresentMode::eFifoRelaxed;

		case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
			return renderer::PresentMode::eSharedDemandRefresh;

		case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
			return renderer::PresentMode::eSharedContinuousRefresh;

		default:
			assert( false && "Unsupported PresentMode" );
			return renderer::PresentMode::eImmediate;
		}
	}
}
