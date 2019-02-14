#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkAttachmentLoadOp convert( ashes::AttachmentLoadOp const & value )
	{
		switch ( value )
		{
		case ashes::AttachmentLoadOp::eLoad:
			return VK_ATTACHMENT_LOAD_OP_LOAD;

		case ashes::AttachmentLoadOp::eClear:
			return VK_ATTACHMENT_LOAD_OP_CLEAR;

		case ashes::AttachmentLoadOp::eDontCare:
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		default:
			assert( false && "Unsupported AttachmentLoadOp" );
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}
	}
}
