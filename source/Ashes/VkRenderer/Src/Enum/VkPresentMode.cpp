#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPresentModeKHR convert( ashes::PresentMode const & value )
	{
		switch ( value )
		{
		case ashes::PresentMode::eImmediate:
			return VK_PRESENT_MODE_IMMEDIATE_KHR;

		case ashes::PresentMode::eMailbox:
			return VK_PRESENT_MODE_MAILBOX_KHR;

		case ashes::PresentMode::eFifo:
			return VK_PRESENT_MODE_FIFO_KHR;

		case ashes::PresentMode::eFifoRelaxed:
			return VK_PRESENT_MODE_FIFO_RELAXED_KHR;

		case ashes::PresentMode::eSharedDemandRefresh:
			return VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;

		case ashes::PresentMode::eSharedContinuousRefresh:
			return VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;

		default:
			assert( false && "Unsupported PresentMode" );
			return VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	ashes::PresentMode convert( VkPresentModeKHR const & value )
	{
		switch ( value )
		{
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			return ashes::PresentMode::eImmediate;

		case VK_PRESENT_MODE_MAILBOX_KHR:
			return ashes::PresentMode::eMailbox;

		case VK_PRESENT_MODE_FIFO_KHR:
			return ashes::PresentMode::eFifo;

		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			return ashes::PresentMode::eFifoRelaxed;

		case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
			return ashes::PresentMode::eSharedDemandRefresh;

		case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
			return ashes::PresentMode::eSharedContinuousRefresh;

		default:
			assert( false && "Unsupported PresentMode" );
			return ashes::PresentMode::eImmediate;
		}
	}
}
