/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_InputAssemblyState_HPP___
#define ___VkRenderer_InputAssemblyState_HPP___
#pragma once

#include <Pipeline/InputAssemblyState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::InputAssemblyState en VkPipelineInputAssemblyStateCreateInfo.
	*\param[in] state
	*	Le ashes::InputAssemblyState.
	*\return
	*	Le VkPipelineInputAssemblyStateCreateInfo.
	*/
	VkPipelineInputAssemblyStateCreateInfo convert( ashes::InputAssemblyState const & state );
}

#endif
