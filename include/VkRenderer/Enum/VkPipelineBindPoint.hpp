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
	*	Convertit un ashes::PipelineBindPoint en VkPipelineBindPoint.
	*\param[in] point
	*	Le ashes::PipelineBindPoint.
	*\return
	*	Le VkPipelineBindPoint.
	*/
	VkPipelineBindPoint convert( ashes::PipelineBindPoint const & point );
}
