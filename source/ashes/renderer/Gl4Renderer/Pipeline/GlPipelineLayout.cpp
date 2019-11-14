#include "Pipeline/GlPipelineLayout.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Pipeline/GlPipeline.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	bool isBuffer( VkDescriptorType type )
	{
		return type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
			|| type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
			|| type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
			|| type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	}

	PipelineLayout::PipelineLayout( VkDevice device
		, VkPipelineLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_setLayouts{ createInfo.pSetLayouts, createInfo.pSetLayouts + createInfo.setLayoutCount }
		, m_pushConstantRanges{ createInfo.pPushConstantRanges, createInfo.pPushConstantRanges + createInfo.pushConstantRangeCount }
		, m_createInfo{ createInfo }
	{
		m_createInfo.pSetLayouts = m_setLayouts.data();
		m_createInfo.pPushConstantRanges = m_pushConstantRanges.data();
		uint32_t set = 0u;
		uint32_t index = 0u;

		for ( auto & descriptorLayout : m_setLayouts )
		{
			for ( auto & binding : *get( descriptorLayout ) )
			{
				if ( isBuffer( binding.descriptorType ) )
				{
					m_shaderBindings.emplace( makeShaderBindingKey( set, binding.binding ), index++ );
				}
			}

			++set;
		}
	}

	ShaderBindingMap const & PipelineLayout::getShaderBindings()const
	{
		return m_shaderBindings;
	}

	uint32_t PipelineLayout::getDescriptorSetIndex( VkDescriptorSet set )const
	{
		auto & layouts = getDescriptorsLayouts();
		auto layout = get( set )->getLayout();
		auto it = std::find( layouts.begin()
			, layouts.end()
			, layout );
		assert( it != layouts.end() );
		return uint32_t( std::distance( layouts.begin(), it ) );
	}
}
