/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/RendererPrerequisites.hpp>
#include <VkLib/FlagCombination.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::SubpassContents en VkSubpassContents.
	*\param[in] value
	*	Le renderer::SubpassContents.
	*\return
	*	Le VkSubpassContents.
	*/
	VkSubpassContents convert( renderer::SubpassContents const & value );
}
