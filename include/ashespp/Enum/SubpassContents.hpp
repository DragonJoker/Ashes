/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SubpassContents_HPP___
#define ___AshesPP_SubpassContents_HPP___
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
	inline std::string getName( VkSubpassContents value )
	{
		switch ( value )
		{
		case VK_SUBPASS_CONTENTS_INLINE:
			return "inline";
		case VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS:
			return "secondaryy_command_buffers";
		default:
			assert( false && "Unsupported VkSubpassContents." );
			throw std::runtime_error{ "Unsupported VkSubpassContents" };
		}

		return 0;
	}
}

#endif
