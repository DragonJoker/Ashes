/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

#include "Pipeline/VkSpecialisationMapEntry.hpp"

namespace vk_renderer
{
	VkSpecializationInfo convert( ashes::SpecialisationInfoBase const & state
		, std::vector< VkSpecializationMapEntry > const & mapEntries )
	{
		return VkSpecializationInfo
		{
			uint32_t( mapEntries.size() ),
			mapEntries.data(),
			uint32_t( state.getSize() ),
			state.getData()
		};
	}
}
