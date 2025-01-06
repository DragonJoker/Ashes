#include "Pipeline/XBoxPipelineLayout.hpp"

#include "Descriptor/XBoxDescriptorSet.hpp"
#include "Descriptor/XBoxDescriptorSetLayout.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	PipelineLayout::PipelineLayout( VkDevice device
		, VkPipelineLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_setLayouts{ createInfo.pSetLayouts, createInfo.pSetLayouts + createInfo.setLayoutCount }
		, m_pushConstantRanges{ createInfo.pPushConstantRanges, createInfo.pPushConstantRanges + createInfo.pushConstantRangeCount }
		, m_createInfo{ std::move( createInfo ) }
	{
		m_createInfo.pSetLayouts = m_setLayouts.data();
		m_createInfo.pPushConstantRanges = m_pushConstantRanges.data();
		uint32_t set = 0u;
		ShaderBindingIndices indices;

		for ( auto & descriptorLayout : m_setLayouts )
		{
			for ( auto & binding : *get( descriptorLayout ) )
			{
				addBinding( set, binding, m_shaderBindings, indices );
			}

			++set;
		}
	}

	PipelineLayout::~PipelineLayout()noexcept
	{
		for ( auto & pipeline : m_pipelines )
		{
			get( pipeline )->m_layout = nullptr;
		}
	}

	ShaderBindings const & PipelineLayout::getShaderBindings()const
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

	void PipelineLayout::addPipeline( VkPipeline pipeline )
	{
		m_pipelines.insert( pipeline );
	}

	void PipelineLayout::removePipeline( VkPipeline pipeline )
	{
		m_pipelines.erase( pipeline );
	}
}
