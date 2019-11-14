/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11DrawIndexedIndirectCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	DrawIndexedIndirectCommand::DrawIndexedIndirectCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, VkIndexType type
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_buffer{ buffer }
		, m_offset{ UINT( offset ) }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
		, m_mode{ getPrimitiveTopology( mode ) }
		, m_type{ getIndexFormat( type ) }
		, m_vbos{ vbos }
	{
	}

	void DrawIndexedIndirectCommand::apply( Context const & context )const
	{
		context.context->IASetPrimitiveTopology( m_mode );

		for ( auto & binding : m_vbos )
		{
			context.context->IASetVertexBuffers( binding.startIndex
				, UINT( binding.d3dBuffers.size() )
				, binding.d3dBuffers.data()
				, binding.strides.data()
				, binding.offsets.data() );
		}

		context.context->DrawIndexedInstancedIndirect( get( m_buffer )->getBuffer()
			, m_offset );
	}

	CommandPtr DrawIndexedIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndexedIndirectCommand >( *this );
	}
}
