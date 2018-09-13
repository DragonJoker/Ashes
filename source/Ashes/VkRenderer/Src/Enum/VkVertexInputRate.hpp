/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::VertexInputRate en VkVertexInputRate.
	*\param[in] value
	*	Le ashes::VertexInputRate.
	*\return
	*	Le VkVertexInputRate.
	*/
	VkVertexInputRate convert( ashes::VertexInputRate const & value );
}
