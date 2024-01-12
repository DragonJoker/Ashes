#include "Pipeline/GlPipelineLayout.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Pipeline/GlPipeline.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	PipelineLayout::PipelineLayout( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkPipelineLayoutCreateInfo const & createInfo )
		: m_device{ device }
		, m_setLayouts{ createInfo.pSetLayouts, createInfo.pSetLayouts + createInfo.setLayoutCount }
		, m_pushConstantRanges{ createInfo.pPushConstantRanges, createInfo.pPushConstantRanges + createInfo.pushConstantRangeCount }
		, m_createInfo{ createInfo }
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

		registerObject( m_device, *this );
	}

	PipelineLayout::~PipelineLayout()noexcept
	{
		unregisterObject( m_device, *this );

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

		if ( it == layouts.end() )
		{
			return GL_INVALID_INDEX;
		}

		return uint32_t( std::distance( layouts.begin(), it ) );
	}

	ShaderBindings const & PipelineLayout::getDescriptorSetBindings( VkDescriptorSet descriptorSet
		, uint32_t descriptorSetIndex )const
	{
		auto key = makeDescriptorKey( descriptorSet, descriptorSetIndex );
		auto [it, res] = m_dsBindings.try_emplace( key );

		if ( res )
		{
			for ( auto & binding : *get( get( descriptorSet )->getLayout() ) )
			{
				copyBinding( descriptorSetIndex, binding, m_shaderBindings, it->second );
			}
		}

		return it->second;
	}

	void PipelineLayout::addPipeline( VkPipeline pipeline )
	{
		m_pipelines.insert( pipeline );
	}

	void PipelineLayout::removePipeline( VkPipeline pipeline )noexcept
	{
		m_pipelines.erase( pipeline );
	}
}
