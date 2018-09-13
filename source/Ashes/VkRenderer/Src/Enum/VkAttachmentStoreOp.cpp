#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkAttachmentStoreOp convert( ashes::AttachmentStoreOp const & value )
	{
		switch ( value )
		{
		case ashes::AttachmentStoreOp::eStore:
			return VK_ATTACHMENT_STORE_OP_STORE;

		case ashes::AttachmentStoreOp::eDontCare:
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;

		default:
			assert( false && "Unsupported AttachmentStoreOp" );
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
	}
}
