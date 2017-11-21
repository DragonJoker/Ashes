/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "DescriptorSetLayoutBinding.hpp"

namespace renderer
{
	DescriptorSetLayoutBinding::DescriptorSetLayoutBinding( uint32_t index
		, DescriptorType type
		, ShaderStageFlags flags )
		: m_binding{ index
			, convert( type )
			, convert( flags ) }
	{
	}
}
