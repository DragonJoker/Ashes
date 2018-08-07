/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ImageUsageFlags en VkImageUsageFlags.
	*\param[in] flags
	*	Le renderer::ImageUsageFlags.
	*\return
	*	Le VkImageUsageFlags.
	*/
	VkImageUsageFlags convert( renderer::ImageUsageFlags const & flags );
	/**
	*\brief
	*	Convertit un VkImageUsageFlags en renderer::ImageUsageFlags.
	*\param[in] flags
	*	Le VkImageUsageFlags.
	*\return
	*	Le renderer::ImageUsageFlags.
	*/
	renderer::ImageUsageFlags convertImageUsageFlags( VkImageUsageFlags const & flags );
}
