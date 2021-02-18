/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlShaderStage
	{
		GL_SHADER_STAGE_FRAGMENT = 0x8B30,
		GL_SHADER_STAGE_VERTEX = 0x8B31,
		GL_SHADER_STAGE_GEOMETRY = 0x8DD9,
		GL_SHADER_STAGE_TESS_CONTROL = 0x8E88,
		GL_SHADER_STAGE_TESS_EVALUATION = 0x8E87,
		GL_SHADER_STAGE_COMPUTE = 0x91B9,
	};
	std::string getShaderStageName( GlShaderStage value );
	inline std::string toString( GlShaderStage value ) { return getShaderStageName( value ); }
	/**
	*\brief
	*	Convertit un VkShaderStageFlagBits en VkShaderStageFlagBits.
	*\param[in] flags
	*	Le VkShaderStageFlagBits.
	*\return
	*	Le GlShaderStageFlag.
	*/
	GlShaderStage getShaderStage( VkShaderStageFlagBits const & flag );
	enum GlShaderStageFlag
	{
		GL_SHADER_STAGE_FLAG_VERTEX = 0x01 << 0,
		GL_SHADER_STAGE_FLAG_FRAGMENT = 0x01 << 1,
		GL_SHADER_STAGE_FLAG_GEOMETRY = 0x01 << 2,
		GL_SHADER_STAGE_FLAG_TESS_CONTROL = 0x01 << 3,
		GL_SHADER_STAGE_FLAG_TESS_EVALUATION = 0x01 << 4,
		GL_SHADER_STAGE_FLAG_COMPUTE = 0x01 << 5,
	};
	Ashes_ImplementFlag( GlShaderStageFlag )
	std::string getShaderStageFlagsName( GlShaderStageFlags value );
	inline std::string toString( GlShaderStageFlags value ) { return getShaderStageFlagsName( value ); }
	/**
	*\brief
	*	Convertit un VkShaderStageFlags en VkShaderStageFlags.
	*\param[in] flags
	*	Le VkShaderStageFlags.
	*\return
	*	Le GlShaderStageFlags.
	*/
	GlShaderStageFlag convert( VkShaderStageFlagBits flag );
	/**
	*\brief
	*	Convertit un VkShaderStageFlags en VkShaderStageFlags.
	*\param[in] flags
	*	Le VkShaderStageFlags.
	*\return
	*	Le GlShaderStageFlags.
	*/
	GlShaderStageFlags convert( VkShaderStageFlags flags );
}
