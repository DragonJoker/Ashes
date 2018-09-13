/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11DrawCommand.hpp"

namespace d3d11_renderer
{
	DrawCommand::DrawCommand( Device const & device
		, uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance
		, renderer::PrimitiveTopology mode
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_vtxCount{ vtxCount }
		, m_instCount{ instCount }
		, m_firstVertex{ firstVertex }
		, m_firstInstance{ firstInstance }
		, m_mode{ convert( mode ) }
		, m_vbos{ vbos }
	{
	}

	void DrawCommand::apply( Context const & context )const
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

		if ( m_instCount > 1 )
		{
			context.context->DrawInstanced( m_vtxCount, m_instCount, m_firstVertex, m_firstInstance );
		}
		else
		{
			context.context->Draw( m_vtxCount, m_firstVertex );
		}
	}

	CommandPtr DrawCommand::clone()const
	{
		return std::make_unique< DrawCommand >( *this );
	}
}
