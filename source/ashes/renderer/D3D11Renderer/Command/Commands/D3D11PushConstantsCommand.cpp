/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11PushConstantsCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Pipeline/PipelineLayout.hpp>
#include <Ashes/Miscellaneous/PushConstantRange.hpp>

namespace ashes::d3d11
{
	PushConstantsCommand::PushConstantsCommand( VkDevice device
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
			checkError( context.device, hr, "Map" );
			std::memcpy( reinterpret_cast< uint8_t * >( mapped.pData ) + m_pcb.data.offset
				, m_pcb.data.data.data()
				, m_pcb.data.size );
			context.context->Unmap( buffer, 0u );

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_VERTEX_BIT ) )
			{
				context.context->VSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT ) )
			{
				context.context->HSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ) )
			{
				context.context->DSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_GEOMETRY_BIT ) )
			{
				context.context->GSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_FRAGMENT_BIT ) )
			{
				context.context->PSSetConstantBuffers( m_pcb.location, 1u, &buffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_COMPUTE_BIT ) )
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

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_VERTEX_BIT ) )
			{
				context.context->VSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT ) )
			{
				context.context->HSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ) )
			{
				context.context->DSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_GEOMETRY_BIT ) )
			{
				context.context->GSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_FRAGMENT_BIT ) )
			{
				context.context->PSSetConstantBuffers( m_pcb.location, 1u, &nullBuffer );
			}

			if ( checkFlag( m_pcb.data.stageFlags, VK_SHADER_STAGE_COMPUTE_BIT ) )
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
