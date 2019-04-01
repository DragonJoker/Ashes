/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Pipeline/PipelineLayout.hpp"

#include "AshesRenderer/Core/Device.hpp"
#include "AshesRenderer/Descriptor/DescriptorSetLayout.hpp"

namespace ashes
{
	PipelineLayout::PipelineLayout( Device const & device
		, DescriptorSetLayoutCRefArray const & layouts
		, PushConstantRangeArray const & pushConstantRanges )
		: m_device{ device }
		, m_pushConstantRanges{ pushConstantRanges }
	{
		registerObject( m_device, "PipelineLayout", this );
	}

	PipelineLayout::~PipelineLayout()
	{
		unregisterObject( m_device, this );
	}
}
