/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ShaderStageFlags en VkShaderStageFlags.
	*\param[in] flags
	*	Le ashes::ShaderStageFlags.
	*\return
	*	Le VkShaderStageFlags.
	*/
	VkShaderStageFlags convert( ashes::ShaderStageFlags const & flags );
	/**
	*\brief
	*	Convertit un ashes::ShaderStageFlag en VkShaderStageFlagBits.
	*\param[in] flags
	*	Le ashes::ShaderStageFlag.
	*\return
	*	Le VkShaderStageFlagBits.
	*/
	VkShaderStageFlagBits convert( ashes::ShaderStageFlag const & flag );
}
