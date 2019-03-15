/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11DrawIndexedCommand.hpp"

namespace d3d11_renderer
{
	namespace
	{
		uint32_t getSize( ashes::IndexType type )
		{
			switch ( type )
			{
			case ashes::IndexType::eUInt16:
				return 2u;
			case ashes::IndexType::eUInt32:
				return 4u;
			default:
				assert( false && "Unsupported index type" );
				return 1u;
			}
		}
	}

	DrawIndexedCommand::DrawIndexedCommand( Device const & device
		, uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance
		, ashes::PrimitiveTopology mode
		, ashes::IndexType type
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_indexCount{ indexCount }
		, m_instCount{ instCount }
		, m_firstIndex{ firstIndex }
		, m_vertexOffset{ vertexOffset }
		, m_firstInstance{ firstInstance }
		, m_mode{ convert( mode ) }
		, m_type{ convert( type ) }
		, m_size{ getSize( type ) }
		, m_vbos{ vbos }
	{
	}

	void DrawIndexedCommand::apply( Context const & context )const
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
			context.context->DrawIndexedInstanced( m_indexCount
				, m_instCount
				, m_firstIndex
				, m_vertexOffset
				, m_firstInstance );
		}
		else
		{
			context.context->DrawIndexed( m_indexCount
				, m_firstIndex
				, m_vertexOffset );
		}
	}

	CommandPtr DrawIndexedCommand::clone()const
	{
		return std::make_unique< DrawIndexedCommand >( *this );
	}
}
