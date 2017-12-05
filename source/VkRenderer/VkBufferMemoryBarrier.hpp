/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/BufferMemoryBarrier.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::BufferMemoryBarrier en VkBufferMemoryBarrier.
	*\param[in] flags
	*	Le renderer::BufferMemoryBarrier.
	*\return
	*	Le VkBufferMemoryBarrier.
	*/
	VkBufferMemoryBarrier convert( renderer::BufferMemoryBarrier const & barrier );
}
