/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AttachmentStoreOp_HPP___
#define ___AshesPP_AttachmentStoreOp_HPP___
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
	inline std::string getName( VkAttachmentStoreOp value )
	{
		switch ( value )
		{
		case VK_ATTACHMENT_STORE_OP_STORE:
			return "store";
		case VK_ATTACHMENT_STORE_OP_DONT_CARE:
			return "dont_care";
		default:
			assert( false && "Unsupported VkAttachmentStoreOp." );
			return "Unsupported VkAttachmentStoreOp";
		}
	}
}

#endif
