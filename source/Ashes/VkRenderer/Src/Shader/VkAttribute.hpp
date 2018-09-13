/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Shader/Attribute.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Attribute en VkVertexInputAttributeDescription.
	*\param[in] value
	*	Le ashes::Attribute.
	*\return
	*	Le VkVertexInputAttributeDescription.
	*/
	VkVertexInputAttributeDescription convert( ashes::Attribute const & value );
}
