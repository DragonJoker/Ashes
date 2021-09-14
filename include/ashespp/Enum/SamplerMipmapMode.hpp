/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MipmapMode_HPP___
#define ___AshesPP_MipmapMode_HPP___
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
	inline std::string getName( VkSamplerMipmapMode value )
	{
		switch ( value )
		{
		case VK_SAMPLER_MIPMAP_MODE_NEAREST:
			return "nearest";
		case VK_SAMPLER_MIPMAP_MODE_LINEAR:
			return "linear";
		default:
			assert( false && "Unsupported VkSamplerMipmapMode." );
			return "Unsupported VkSamplerMipmapMode";
		}
	}
}

#endif
