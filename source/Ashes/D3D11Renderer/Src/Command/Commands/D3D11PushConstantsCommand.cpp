/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11PushConstantsCommand.hpp"

#include "Buffer/D3D11PushConstantsBuffer.hpp"

namespace d3d11_renderer
{
	PushConstantsCommand::PushConstantsCommand( Device const & device
		, ashes::PipelineLayout const & layout
		, ashes::PushConstantsBufferBase const & pcb )
		: CommandBase{ device }
		, m_pcb{ static_cast< PushConstantsBuffer const & >( pcb ) }
		, m_data{ pcb.getData(), pcb.getData() + pcb.getSize() }
	{
	}

	void PushConstantsCommand::apply( Context const & context )const
	{
		auto buffer = m_pcb.getBuffer();
		D3D11_MAPPED_SUBRESOURCE mapped;
		auto hr = context.context->Map( buffer
			, 0u
			, D3D11_MAP_WRITE_DISCARD
			, 0u
			, &mapped );

		if ( dxCheckError( hr, "Map" ) )
		{
			std::memcpy( mapped.pData
				, m_pcb.getData()
				, m_pcb.getSize() );
			context.context->Unmap( buffer, 0u );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eVertex ) )
		{
			context.context->VSSetConstantBuffers( m_pcb.getLocation(), 1u, &buffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eTessellationControl ) )
		{
			context.context->HSSetConstantBuffers( m_pcb.getLocation(), 1u, &buffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eTessellationEvaluation ) )
		{
			context.context->DSSetConstantBuffers( m_pcb.getLocation(), 1u, &buffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eGeometry ) )
		{
			context.context->GSSetConstantBuffers( m_pcb.getLocation(), 1u, &buffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eFragment ) )
		{
			context.context->PSSetConstantBuffers( m_pcb.getLocation(), 1u, &buffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eCompute ) )
		{
			context.context->CSSetConstantBuffers( m_pcb.getLocation(), 1u, &buffer );
		}
	}

	void PushConstantsCommand::remove( Context const & context )const
	{
		static ID3D11Buffer * nullBuffer{ nullptr };

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eVertex ) )
		{
			context.context->VSSetConstantBuffers( m_pcb.getLocation(), 1u, &nullBuffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eTessellationControl ) )
		{
			context.context->HSSetConstantBuffers( m_pcb.getLocation(), 1u, &nullBuffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eTessellationEvaluation ) )
		{
			context.context->DSSetConstantBuffers( m_pcb.getLocation(), 1u, &nullBuffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eGeometry ) )
		{
			context.context->GSSetConstantBuffers( m_pcb.getLocation(), 1u, &nullBuffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eFragment ) )
		{
			context.context->PSSetConstantBuffers( m_pcb.getLocation(), 1u, &nullBuffer );
		}

		if ( checkFlag( m_pcb.getStageFlags(), ashes::ShaderStageFlag::eCompute ) )
		{
			context.context->CSSetConstantBuffers( m_pcb.getLocation(), 1u, &nullBuffer );
		}
	}

	CommandPtr PushConstantsCommand::clone()const
	{
		return std::make_unique< PushConstantsCommand >( *this );
	}
}
