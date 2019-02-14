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
	*	Convertit un ashes::SampleCountFlag en VkSampleCountFlagBits.
	*\param[in] flags
	*	Le ashes::SampleCountFlag.
	*\return
	*	Le VkSampleCountFlagBits.
	*/
	VkSampleCountFlagBits convert( ashes::SampleCountFlag const & flags );
	/**
	*\brief
	*	Convertit un VkSampleCountFlags en ashes::SampleCountFlags.
	*\param[in] flags
	*	Le VkSampleCountFlags.
	*\return
	*	Le ashes::SampleCountFlags.
	*/
	ashes::SampleCountFlags convertSampleCountFlags( VkSampleCountFlags const & flags );
}
