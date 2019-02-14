/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/SpecialisationMapEntry.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SpecialisationMapEntry en VkSpecializationMapEntry.
	*\param[in] state
	*	Le ashes::SpecialisationMapEntry.
	*\return
	*	Le VkSpecializationMapEntry.
	*/
	VkSpecializationMapEntry convert( ashes::SpecialisationMapEntry const & state );
}
