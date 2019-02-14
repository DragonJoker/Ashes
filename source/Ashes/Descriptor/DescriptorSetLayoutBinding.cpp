/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Descriptor/DescriptorSetLayoutBinding.hpp"

namespace ashes
{
	DescriptorSetLayoutBinding::DescriptorSetLayoutBinding( uint32_t index
		, DescriptorType type
		, ShaderStageFlags flags
		, uint32_t count )
		: m_index{ index }
		, m_type{ type }
		, m_flags{ flags }
		, m_count{ count }
	{
	}
}
