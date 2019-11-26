/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBindVertexBuffersCommand.hpp"

#include "Buffer/TestBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	namespace
	{
		std::vector< uint32_t > convert( UInt64Array const & array )
		{
			std::vector< uint32_t > result;

			for ( auto & v : array )
			{
				result.push_back( uint32_t( v ) );
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
		, m_offsets{ convert( offsets ) }
		, m_strides( m_offsets.size(), 24u )
	{
	}

	void BindVertexBuffersCommand::apply()const
	{
	}

	CommandPtr BindVertexBuffersCommand::clone()const
	{
		return std::make_unique< BindVertexBuffersCommand >( *this );
	}
}
