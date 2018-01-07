/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Renderer/DescriptorSetLayoutBinding.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::DescriptorSetLayoutBinding en vk::DescriptorLayoutBinding.
	*\param[in] flags
	*	Le renderer::DescriptorSetLayoutBinding.
	*\return
	*	Le vk::DescriptorLayoutBinding.
	*/
	VkDescriptorSetLayoutBinding convert( renderer::DescriptorSetLayoutBinding const & binding );
}
