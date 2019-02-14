/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSpecializationMapEntry convert( ashes::SpecialisationMapEntry const & state )
	{
		return VkSpecializationMapEntry
		{
			state.constantID,
			state.offset,
			state.size,
		};
	}
}
