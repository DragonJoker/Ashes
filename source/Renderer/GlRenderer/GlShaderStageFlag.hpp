/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/Enum/ShaderStageFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ShaderStageFlags en VkShaderStageFlags.
	*\param[in] flags
	*	Le renderer::ShaderStageFlags.
	*\return
	*	Le VkShaderStageFlags.
	*/
	GLenum convert( renderer::ShaderStageFlags const & flags );
	/**
	*\brief
	*	Convertit un renderer::ShaderStageFlag en VkShaderStageFlagBits.
	*\param[in] flags
	*	Le renderer::ShaderStageFlag.
	*\return
	*	Le VkShaderStageFlagBits.
	*/
	GLenum convert( renderer::ShaderStageFlag const & flag );
}
