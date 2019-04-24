#include "Pipeline/D3D11ComputePipeline.hpp"

#include "Core/D3D11Device.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/Pipeline/SpecialisationInfo.hpp>

namespace ashes::d3d11
{
	ComputePipeline::ComputePipeline( VkDevice device
		, ashes::PipelineLayout const & layout
		, ashes::ComputePipelineCreateInfo createInfo )
		: ashes::ComputePipeline{ device
			, layout
			, std::move( createInfo ) }
	{
		doCompileProgram( device );
	}

	PushConstantsBuffer ComputePipeline::findPushConstantBuffer( PushConstantsDesc const & pushConstants )const
	{
		// Try to find a PCB that has the same flags, and the same size as the push constants.
		auto it = std::find_if( m_constantsPcbs.begin()
			, m_constantsPcbs.end()
			, [&pushConstants]( PushConstantsBuffer const & lookup )
			{
				return lookup.data.stageFlags == pushConstants.stageFlags
					&& lookup.data.size == pushConstants.offset + pushConstants.size;
			} );

		if ( it == m_constantsPcbs.end() )
		{
			// Try a PCB that has the same flags, but is larger than the push constants.
			it = std::find_if( m_constantsPcbs.begin()
				, m_constantsPcbs.end()
				, [&pushConstants]( PushConstantsBuffer const & lookup )
				{
					return lookup.data.stageFlags == pushConstants.stageFlags
						&& lookup.data.size > pushConstants.offset + pushConstants.size;
				} );
		}

		if ( it == m_constantsPcbs.end() )
		{
			// Try a PCB that contains the flags of the push constants.
			it = std::find_if( m_constantsPcbs.begin()
				, m_constantsPcbs.end()
				, [&pushConstants]( PushConstantsBuffer const & lookup )
				{
					return checkFlag( lookup.data.stageFlags, pushConstants.stageFlags )
						&& lookup.data.size == pushConstants.offset + pushConstants.size;
				} );
		}

		if ( it == m_constantsPcbs.end() )
		{
			// Try a PCB that contains the flags of the push constants, and is larger than them.
			it = std::find_if( m_constantsPcbs.begin()
				, m_constantsPcbs.end()
				, [&pushConstants]( PushConstantsBuffer const & lookup )
				{
					return checkFlag( lookup.data.stageFlags, pushConstants.stageFlags )
						&& lookup.data.size > pushConstants.offset + pushConstants.size;
				} );
		}

		if ( it != m_constantsPcbs.end() )
		{
			return PushConstantsBuffer
			{
				it->ubo,
				it->location,
				pushConstants
			};
		}

		static PushConstantsBuffer const dummy{};
		return dummy;
	}

	void ComputePipeline::doCompileProgram( VkDevice device )
	{
		auto module = std::static_pointer_cast< ShaderModule >( m_createInfo.stage.module );
		m_compiled.push_back( module->compile( m_createInfo.stage ) );
		m_programLayout.emplace( module->getStage(), getProgram().getLayout() );

		for ( auto & shaderLayoutIt : m_programLayout )
		{
			for ( auto & blockLayout : shaderLayoutIt.second.interfaceBlockLayout )
			{
				PushConstantsBuffer pcb
				{
					std::make_shared< ashes::UniformBufferBase >( device
						, 1u
						, blockLayout.size
						, 0u ),
					blockLayout.binding,
					{
						shaderLayoutIt.first,
						0u,
						blockLayout.size
					}
				};
				auto requirements = pcb.ubo->getMemoryRequirements();
				auto deduced = deduceMemoryType( requirements.memoryTypeBits
					, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
				pcb.ubo->bindMemory( device.allocateMemory( { requirements.size, deduced } ) );
				m_constantsPcbs.push_back( std::move( pcb ) );
			}
		}
	}
}
