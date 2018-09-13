#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkQueueFlags convert( ashes::QueueFlags const & flags )
	{
		VkQueueFlags result{ 0 };

		if ( checkFlag( flags, ashes::QueueFlag::eGraphics ) )
		{
			result |= VK_QUEUE_GRAPHICS_BIT;
		}

		if ( checkFlag( flags, ashes::QueueFlag::eCompute ) )
		{
			result |= VK_QUEUE_COMPUTE_BIT;
		}

		if ( checkFlag( flags, ashes::QueueFlag::eTransfer ) )
		{
			result |= VK_QUEUE_TRANSFER_BIT;
		}

		if ( checkFlag( flags, ashes::QueueFlag::eSparseBinding ) )
		{
			result |= VK_QUEUE_SPARSE_BINDING_BIT;
		}

		return result;
	}

	ashes::QueueFlags convertQueueFlags( VkQueueFlags const & flags )
	{
		ashes::QueueFlags result;

		if ( ashes::checkFlag( flags, VK_QUEUE_GRAPHICS_BIT ) )
		{
			result |= ashes::QueueFlag::eGraphics;
		}

		if ( ashes::checkFlag( flags, VK_QUEUE_COMPUTE_BIT ) )
		{
			result |= ashes::QueueFlag::eCompute;
		}

		if ( ashes::checkFlag( flags, VK_QUEUE_TRANSFER_BIT ) )
		{
			result |= ashes::QueueFlag::eTransfer;
		}

		if ( ashes::checkFlag( flags, VK_QUEUE_SPARSE_BINDING_BIT ) )
		{
			result |= ashes::QueueFlag::eSparseBinding;
		}

		return result;
	}
}
