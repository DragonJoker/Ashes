/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

#include "VkSpecialisationMapEntry.hpp"

namespace vk_renderer
{
	VkSpecializationInfo convert( renderer::SpecialisationInfo const & state
		, std::vector< VkSpecializationMapEntry > const & mapEntries )
	{
		return VkSpecializationInfo
		{
			uint32_t( mapEntries.size() ),
			mapEntries.data(),
			uint32_t( state.data.size() ),
			state.data.data()
		};
	}
}
