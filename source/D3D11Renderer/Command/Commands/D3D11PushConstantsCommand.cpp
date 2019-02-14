/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11PushConstantsCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11UniformBuffer.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Ashes/Pipeline/PipelineLayout.hpp>
#include <Ashes/Miscellaneous/PushConstantRange.hpp>

namespace d3d11_renderer
{
	PushConstantsCommand::PushConstantsCommand( Device const & device
		, PushConstantsBuffer const & pcb )
		: CommandBase{ device }
		, m_pcb{ pcb }
	{
	}

	void PushConstantsCommand::apply( Context const & context )const
	{
		if ( m_pcb.ubo )
		{
			auto buffer = static_cast< Buffer const & >( m_pcb.ubo->getBuffer() ).getBuffer();
			D3D11_MAPPED_SUBRESOURCE mapped;
			auto hr = context.context->Map( buffer
				, 0u
				, D3D11_MAP_WRITE_DISCARD
				, 0u
				, &mapped );
			dxCheckError( hr, "Map" );
			std::memcpy( reinterpret_cast< uint8_t * >( mapped.pData ) + m_pcb.data.offset
				, m_pcb.data.data.data()
				, m_pcb.data.size );
			context.context->Unmap( buffer, 0u );

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eVertex ) )
			{
				context.context->VSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eTessellationControl ) )
			{
				context.context->HSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eTessellationEvaluation ) )
			{
				context.context->DSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eGeometry ) )
			{
				context.context->GSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eFragment ) )
			{
				context.context->PSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eCompute ) )
			{
				context.context->CSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}
		}
	}

	void PushConstantsCommand::remove( Context const & context )const
	{
		if ( m_pcb.ubo )
		{
			static ID3D11Buffer * nullBuffer{ nullptr };

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eVertex ) )
			{
				context.context->VSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eTessellationControl ) )
			{
				context.context->HSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eTessellationEvaluation ) )
			{
				context.context->DSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eGeometry ) )
			{
				context.context->GSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eFragment ) )
			{
				context.context->PSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, ashes::ShaderStageFlag::eCompute ) )
			{
				context.context->CSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}
		}
	}

	CommandPtr PushConstantsCommand::clone()const
	{
		return std::make_unique< PushConstantsCommand >( *this );
	}
}
