/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SurfaceTransformFlags en VkSurfaceTransformFlagsKHR.
	*\param[in] flags
	*	Le ashes::SurfaceTransformFlags.
	*\return
	*	Le VkSurfaceTransformFlagsKHR.
	*/
	VkSurfaceTransformFlagBitsKHR convert( ashes::SurfaceTransformFlag const & value );
	/**
	*\brief
	*	Convertit un VkSurfaceTransformFlagBitsKHR en ashes::SurfaceTransformFlag.
	*\param[in] flags
	*	Le VkSurfaceTransformFlagBitsKHR.
	*\return
	*	Le ashes::SurfaceTransformFlag.
	*/
	ashes::SurfaceTransformFlag convert( VkSurfaceTransformFlagBitsKHR const & value );
	/**
	*\brief
	*	Convertit un ashes::SurfaceTransformFlags en VkSurfaceTransformFlagsKHR.
	*\param[in] flags
	*	Le ashes::SurfaceTransformFlags.
	*\return
	*	Le VkSurfaceTransformFlagsKHR.
	*/
	VkSurfaceTransformFlagsKHR convert( ashes::SurfaceTransformFlags const & flags );
	/**
	*\brief
	*	Convertit un VkSurfaceTransformFlagsKHR en ashes::SurfaceTransformFlags.
	*\param[in] flags
	*	Le VkSurfaceTransformFlagsKHR.
	*\return
	*	Le ashes::SurfaceTransformFlags.
	*/
	ashes::SurfaceTransformFlags convertSurfaceTransform( VkSurfaceTransformFlagsKHR const & flags );
}
