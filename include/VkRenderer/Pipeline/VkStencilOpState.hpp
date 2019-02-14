/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/StencilOpState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::StencilOpState en VkStencilOpState.
	*\param[in] state
	*	Le ashes::StencilOpState.
	*\return
	*	Le VkStencilOpState.
	*/
	VkStencilOpState convert( ashes::StencilOpState const & state );
}
