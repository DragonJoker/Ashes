/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/SpecialisationInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SpecialisationInfo en VkSpecializationInfo.
	*\param[in] state
	*	Le ashes::SpecialisationInfo.
	*\return
	*	Le VkSpecializationInfo.
	*/
	VkSpecializationInfo convert( ashes::SpecialisationInfoBase const & state
		, std::vector< VkSpecializationMapEntry > const & mapEntries );
}
