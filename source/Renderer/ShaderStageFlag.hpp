/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Masques de bits décrivant les niveaux de shader.
	*/
	enum class ShaderStageFlag
		: uint32_t
	{
		eVertex = 0x00000001,
		eTessellationControl = 0x00000002,
		eTessellationEvaluation = 0x00000004,
		eGeometry = 0x00000008,
		eFragment = 0x00000010,
		eCompute = 0x00000020,
		eAll = 0x0000001F,
	};
	VkLib_ImplementFlag( ShaderStageFlag )
	/**
	*\brief
	*	Convertit un renderer::ShaderStageFlags en VkShaderStageFlags.
	*\param[in] flags
	*	Le renderer::ShaderStageFlags.
	*\return
	*	Le VkShaderStageFlags.
	*/
	VkShaderStageFlags convert( ShaderStageFlags const & flags );
	/**
	*\brief
	*	Convertit un renderer::ShaderStageFlag en VkShaderStageFlagBits.
	*\param[in] flags
	*	Le renderer::ShaderStageFlag.
	*\return
	*	Le VkShaderStageFlagBits.
	*/
	VkShaderStageFlagBits convert( ShaderStageFlag const & flag );
}
