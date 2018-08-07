/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::SurfaceTransformFlags en VkSurfaceTransformFlagsKHR.
	*\param[in] flags
	*	Le renderer::SurfaceTransformFlags.
	*\return
	*	Le VkSurfaceTransformFlagsKHR.
	*/
	VkSurfaceTransformFlagBitsKHR convert( renderer::SurfaceTransformFlag const & value );
	/**
	*\brief
	*	Convertit un VkSurfaceTransformFlagBitsKHR en renderer::SurfaceTransformFlag.
	*\param[in] flags
	*	Le VkSurfaceTransformFlagBitsKHR.
	*\return
	*	Le renderer::SurfaceTransformFlag.
	*/
	renderer::SurfaceTransformFlag convert( VkSurfaceTransformFlagBitsKHR const & value );
	/**
	*\brief
	*	Convertit un renderer::SurfaceTransformFlags en VkSurfaceTransformFlagsKHR.
	*\param[in] flags
	*	Le renderer::SurfaceTransformFlags.
	*\return
	*	Le VkSurfaceTransformFlagsKHR.
	*/
	VkSurfaceTransformFlagsKHR convert( renderer::SurfaceTransformFlags const & flags );
	/**
	*\brief
	*	Convertit un VkSurfaceTransformFlagsKHR en renderer::SurfaceTransformFlags.
	*\param[in] flags
	*	Le VkSurfaceTransformFlagsKHR.
	*\return
	*	Le renderer::SurfaceTransformFlags.
	*/
	renderer::SurfaceTransformFlags convertSurfaceTransform( VkSurfaceTransformFlagsKHR const & flags );
}
