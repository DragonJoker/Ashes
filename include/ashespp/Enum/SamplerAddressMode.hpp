/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_WrapMode_HPP___
#define ___AshesPP_WrapMode_HPP___
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
	inline std::string getName( VkSamplerAddressMode value )
	{
		switch ( value )
		{
		case VK_SAMPLER_ADDRESS_MODE_REPEAT:
			return "repeat";
		case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
			return "mirrored_repeat";
		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:
			return "clamp_to_edge";
		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:
			return "clamp_to_border";
		case VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE:
			return "mirrored_clamp_to_edge";
		default:
			assert( false && "Unsupported VkSamplerAddressMode." );
			return "Unsupported VkSamplerAddressMode";
		}
	}
}

#endif
