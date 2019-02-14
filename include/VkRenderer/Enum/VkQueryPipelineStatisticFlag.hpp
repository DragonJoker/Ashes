/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::QueryPipelineStatisticFlags en VkQueryPipelineStatisticFlags.
	*\param[in] flags
	*	Le ashes::QueryPipelineStatisticFlags.
	*\return
	*	Le VkQueryPipelineStatisticFlags.
	*/
	VkQueryPipelineStatisticFlags convert( ashes::QueryPipelineStatisticFlags const & flags );
}
