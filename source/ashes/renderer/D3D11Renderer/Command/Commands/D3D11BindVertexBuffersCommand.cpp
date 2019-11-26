/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindVertexBuffersCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		std::vector< ID3D11Buffer * > convert( VkBufferArray const & vbos )
		{
			std::vector< ID3D11Buffer * > result;

			for ( auto & vbo : vbos )
			{
				result.push_back( get( vbo )->getBuffer() );
			}

			return result;
		}

		std::vector< UINT > convert( UInt64Array const & array )
		{
			std::vector< UINT > result;

			for ( auto & v : array )
			{
				result.push_back( UINT( v ) );
			}

			return result;
		}
	}

	BindVertexBuffersCommand::BindVertexBuffersCommand( VkDevice device
		, uint32_t firstBinding
		, VkBufferArray const & buffers
		, UInt64Array const & offsets )
		: CommandBase{ device }
		, m_firstBinding{ firstBinding }
		, m_vbos{ buffers }
		, m_buffers{ convert( m_vbos ) }
		, m_offsets{ convert( offsets ) }
		, m_strides( m_offsets.size(), 24u )
	{
	}

	void BindVertexBuffersCommand::remove( Context const & context )const
	{
		std::vector< ID3D11Buffer * > buffers( m_buffers.size(), nullptr );
		std::vector< UINT > offsets( m_buffers.size(), 0u );
		std::vector< UINT > strides( m_buffers.size(), 0u );
		context.context->IASetVertexBuffers( m_firstBinding
			, UINT( buffers.size() )
			, buffers.data()
			, strides.data()
			, offsets.data() );
	}

	void BindVertexBuffersCommand::apply( Context const & context )const
	{
		context.context->IASetVertexBuffers( m_firstBinding
			, UINT( m_buffers.size() )
			, m_buffers.data()
			, m_strides.data()
			, m_offsets.data() );
	}

	CommandPtr BindVertexBuffersCommand::clone()const
	{
		return std::make_unique< BindVertexBuffersCommand >( *this );
	}
}
