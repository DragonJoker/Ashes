/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_FramebufferCreateFlags_HPP___
#define ___AshesPP_FramebufferCreateFlags_HPP___
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
	inline std::string getName( VkFramebufferCreateFlagBits value )
	{
		switch ( value )
		{
#if defined( VK_API_VERSION_1_2 )
		case VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT:
			return "imageless";
#elif defined( VK_KHR_imageless_framebuffer )
		case VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT_KHR:
			return "imageless";
#endif
		default:
			assert( false && "Unsupported VkFramebufferCreateFlagBits." );
			return "Unsupported VkFramebufferCreateFlagBits";
		}
	}
}

#endif
