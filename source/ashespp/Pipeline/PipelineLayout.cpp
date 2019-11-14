/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Pipeline/PipelineLayout.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Descriptor/DescriptorSetLayout.hpp"
#include "ashespp/Pipeline/ComputePipeline.hpp"
#include "ashespp/Pipeline/GraphicsPipeline.hpp"

namespace ashes
{
	PipelineLayout::PipelineLayout( Device const & device
		, DescriptorSetLayoutCRefArray const & layouts
		, VkPushConstantRangeArray const & pushConstantRanges )
		: m_device{ device }
		, m_setLayouts{ layouts }
		, m_pushConstantRanges{ pushConstantRanges }
	{
		auto vklayouts = makeVkArray< VkDescriptorSetLayout >( m_setLayouts );
		VkPipelineLayoutCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			nullptr,
			0,                                                            // flags
			uint32_t( vklayouts.size() ),                                 // setLayoutCount
			( vklayouts.empty()                                           // pSetLayouts
				? nullptr
				: vklayouts.data() ),
			uint32_t( m_pushConstantRanges.size() ),                      // pushConstantRangeCount
			( m_pushConstantRanges.empty()                                // pPushConstantRanges
				? nullptr
				: m_pushConstantRanges.data() )
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreatePipelineLayout( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "PipelineLayout creation" );
		registerObject( m_device, "PipelineLayout", this );
	}

	PipelineLayout::~PipelineLayout()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyPipelineLayout( m_device
			, m_internal
			, nullptr );
	}
}
