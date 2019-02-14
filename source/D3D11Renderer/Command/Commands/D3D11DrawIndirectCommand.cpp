/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11DrawIndirectCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

namespace d3d11_renderer
{
	DrawIndirectCommand::DrawIndirectCommand( Device const & device
		, ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride
		, ashes::PrimitiveTopology mode
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_buffer{ static_cast< Buffer const & >( buffer ) }
		, m_offset{ offset }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
		, m_mode{ convert( mode ) }
		, m_vbos{ vbos }
	{
	}

	void DrawIndirectCommand::apply( Context const & context )const
	{
		context.context->IASetPrimitiveTopology( m_mode );

		for ( auto & binding : m_vbos )
		{
			context.context->IASetVertexBuffers( binding.startIndex
				, UINT( binding.buffers.size() )
				, binding.buffers.data()
				, binding.strides.data()
				, binding.offsets.data() );
		}

		context.context->DrawInstancedIndirect( m_buffer.getBuffer()
			, m_offset );
	}

	CommandPtr DrawIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndirectCommand >( *this );
	}
}
