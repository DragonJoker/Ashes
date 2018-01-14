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
	*	Convertit un renderer::DescriptorSetLayoutBinding en VkDescriptorSetLayoutBinding.
	*\param[in] flags
	*	Le renderer::DescriptorSetLayoutBinding.
	*\return
	*	Le VkDescriptorSetLayoutBinding.
	*/
	VkDescriptorSetLayoutBinding convert( renderer::DescriptorSetLayoutBinding const & binding );
}
