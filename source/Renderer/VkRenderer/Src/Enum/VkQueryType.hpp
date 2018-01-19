/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/QueryType.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::QueryType en VkQueryType.
	*\param[in] value
	*	Le renderer::QueryType.
	*\return
	*	Le VkQueryType.
	*/
	VkQueryType convert( renderer::QueryType const & value );
}
