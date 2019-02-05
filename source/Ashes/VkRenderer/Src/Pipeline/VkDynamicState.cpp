/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPipelineDynamicStateEnableCreateInfo convert( ashes::DynamicState const & state
		, std::vector< VkDynamicStateEnable > & enables )
	{
		enables = convert< VkDynamicStateEnable >( state.dynamicStates );
		return VkPipelineDynamicStateEnableCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			nullptr,
			convert( state.flags ),
			uint32_t( enables.size() ),
			enables.data()
		};
	}
}
