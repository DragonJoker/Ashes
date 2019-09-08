/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SurfaceTransformFlag_HPP___
#define ___AshesPP_SurfaceTransformFlag_HPP___
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
	inline std::string getName( VkSurfaceTransformFlagBitsKHR value )
	{
		switch ( value )
		{
		case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:
			return "identity";
		case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR:
			return "rotate_90";
		case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR:
			return "rotate_180";
		case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR:
			return "rotate_270";
		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR:
			return "horizontal_mirror";
		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR:
			return "horizontal_mirror_rotate_90";
		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR:
			return "horizontal_mirror_rotate_180";
		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR:
			return "horizontal_mirror_rotate_270";
		case VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR:
			return "inherit";
		default:
			assert( false && "Unsupported VkStencilOp." );
			throw std::runtime_error{ "Unsupported VkStencilOp" };
		}

		return 0;
	}
}

#endif
