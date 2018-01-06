/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/BlendOp.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::BlendOp en VkBlendOp.
	*\param[in] value
	*	Le renderer::BlendOp.
	*\return
	*	Le VkBlendOp.
	*/
	VkBlendOp convert( renderer::BlendOp const & value );
}
