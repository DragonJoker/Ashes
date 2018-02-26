/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/SpecialisationInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::SpecialisationInfo en VkSpecializationInfo.
	*\param[in] state
	*	Le renderer::SpecialisationInfo.
	*\return
	*	Le VkSpecializationInfo.
	*/
	VkSpecializationInfo convert( renderer::SpecialisationInfo const & state
		, std::vector< VkSpecializationMapEntry > const & mapEntries );
}
