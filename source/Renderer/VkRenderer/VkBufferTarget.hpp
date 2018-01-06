/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/BufferTarget.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::BufferTargets en VkBufferUsageFlags.
	*\param[in] targets
	*	Le renderer::BufferTargets.
	*\return
	*	Le VkBufferUsageFlags.
	*/
	VkBufferUsageFlags convert( renderer::BufferTargets const & targets );
}
