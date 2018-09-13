/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11DrawIndexedIndirectCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

namespace d3d11_renderer
{
	DrawIndexedIndirectCommand::DrawIndexedIndirectCommand( Device const & device
		, renderer::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride
		, renderer::PrimitiveTopology mode
		, renderer::IndexType type
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_buffer{ static_cast< Buffer const & >( buffer ) }
		, m_offset{ offset }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
		, m_mode{ convert( mode ) }
		, m_type{ convert( type ) }
		, m_vbos{ vbos }
	{
	}

	void DrawIndexedIndirectCommand::apply( Context const & context )const
	{
		for ( auto & binding : m_vbos )
		{
			context.context->IASetVertexBuffers( binding.startIndex
				, UINT( binding.buffers.size() )
				, binding.buffers.data()
				, binding.strides.data()
				, binding.offsets.data() );
		}
	}

	CommandPtr DrawIndexedIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndexedIndirectCommand >( *this );
	}
}
