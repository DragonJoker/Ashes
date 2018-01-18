/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/Enum/SampleCountFlag.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::SampleCountFlag en VkSampleCountFlagBits.
	*\param[in] flags
	*	Le renderer::SampleCountFlag.
	*\return
	*	Le VkSampleCountFlagBits.
	*/
	VkSampleCountFlagBits convert( renderer::SampleCountFlag const & flags );
}
