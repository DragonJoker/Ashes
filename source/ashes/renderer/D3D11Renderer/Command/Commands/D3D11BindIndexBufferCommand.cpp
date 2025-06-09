/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindIndexBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	BindIndexBufferCommand::BindIndexBufferCommand( VkDevice device
		, VkBuffer ibo
		, uint64_t offset
		, VkIndexType indexType )
		: CommandBase{ device }
		, m_ibo{ ibo }
		, m_offset{ UINT( offset ) }
		, m_indexType{ getIndexFormat( indexType ) }
	{
	}

	void BindIndexBufferCommand::apply( Context const & context )const
	{
		context.context->IASetIndexBuffer( get( m_ibo )->getBuffer()
			, m_indexType
			, m_offset );
	}

	void BindIndexBufferCommand::remove( Context const & context )const
	{
		context.context->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0u );
	}

	CommandPtr BindIndexBufferCommand::clone()const
	{
		return std::make_unique< BindIndexBufferCommand >( *this );
	}
}
