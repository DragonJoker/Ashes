/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl4
{
	enum GlShaderStageFlag
	{
		GL_SHADER_STAGE_FRAGMENT = 0x8B30,
		GL_SHADER_STAGE_VERTEX = 0x8B31,
		GL_SHADER_STAGE_GEOMETRY = 0x8DD9,
		GL_SHADER_STAGE_TESS_CONTROL = 0x8E88,
		GL_SHADER_STAGE_TESS_EVALUATION = 0x8E87,
		GL_SHADER_STAGE_COMPUTE = 0x91B9,
	};
	Ashes_ImplementFlag( GlShaderStageFlag )
	std::string getShaderStageFlagName( GlShaderStageFlags value );
	std::string getShaderStageFlagName( VkShaderStageFlags value );
	/**
	*\brief
	*	Convertit un VkShaderStageFlags en VkShaderStageFlags.
	*\param[in] flags
	*	Le VkShaderStageFlags.
	*\return
	*	Le GlShaderStageFlags.
	*/
	GlShaderStageFlags convertShaderStageFlags( VkShaderStageFlags const & flags );
	/**
	*\brief
	*	Convertit un VkShaderStageFlagBits en VkShaderStageFlagBits.
	*\param[in] flags
	*	Le VkShaderStageFlagBits.
	*\return
	*	Le GlShaderStageFlag.
	*/
	GlShaderStageFlag convertShaderStageFlag( VkShaderStageFlagBits const & flag );
}
