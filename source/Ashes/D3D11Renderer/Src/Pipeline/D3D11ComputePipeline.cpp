#include "Pipeline/D3D11ComputePipeline.hpp"

#include "Buffer/D3D11UniformBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Pipeline/SpecialisationInfo.hpp>

namespace d3d11_renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, ashes::PipelineLayout const & layout
		, ashes::ComputePipelineCreateInfo && createInfo )
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

	void ComputePipeline::doCompileProgram( Device const & device )
	{
		auto module = std::static_pointer_cast< ShaderModule >( m_createInfo.stage.module );
		m_programLayout.emplace( module->getStage(), module->compile( m_createInfo.stage ) );

		for ( auto & shaderLayoutIt : m_programLayout )
		{
			for ( auto & blockLayout : shaderLayoutIt.second.interfaceBlockLayout )
			{
				PushConstantsBuffer pcb
				{
					std::make_shared< UniformBuffer >( device
					, 1u
						, blockLayout.size
						, 0u
						, ashes::MemoryPropertyFlag::eHostVisible ),
					blockLayout.binding,
				{
					shaderLayoutIt.first,
					0u,
					blockLayout.size
				}
				};
				m_constantsPcbs.push_back( std::move( pcb ) );
			}
		}
	}
}
