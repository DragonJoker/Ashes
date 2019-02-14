/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::IndexType en VkIndexType.
	*\param[in] type
	*	Le ashes::IndexType.
	*\return
	*	Le VkIndexType.
	*/
	VkIndexType convert( ashes::IndexType const & type );
}
