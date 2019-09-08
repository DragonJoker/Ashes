/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AttachmentLoadOp_HPP___
#define ___AshesPP_AttachmentLoadOp_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkAttachmentLoadOp value )
	{
		switch ( value )
		{
		case VK_ATTACHMENT_LOAD_OP_LOAD:
			return "load";
		case VK_ATTACHMENT_LOAD_OP_CLEAR:
			return "clear";
		case VK_ATTACHMENT_LOAD_OP_DONT_CARE:
			return "dont_care";
		default:
			assert( false && "Unsupported VkAttachmentLoadOp." );
			throw std::runtime_error{ "Unsupported VkAttachmentLoadOp" };
		}

		return 0;
	}
}

#endif
