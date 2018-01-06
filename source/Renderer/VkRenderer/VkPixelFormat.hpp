/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Utils/PixelFormat.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un utils::PixelFormat en VkFormat.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	Le VkFormat.
	*/
	VkFormat convert( utils::PixelFormat const & format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en utils::PixelFormat.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le utils::PixelFormat.
	*/
	utils::PixelFormat convert( VkFormat const & format )noexcept;
}
