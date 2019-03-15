/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindIndexBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

namespace d3d11_renderer
{
	BindIndexBufferCommand::BindIndexBufferCommand( Device const & device
		, Buffer const & ibo
		, uint64_t offset
		, ashes::IndexType indexType )
		: CommandBase{ device }
		, m_ibo{ ibo }
		, m_offset{ UINT( offset ) }
		, m_indexType{ convert( indexType ) }
	{
	}

	void BindIndexBufferCommand::apply( Context const & context )const
	{
		context.context->IASetIndexBuffer( m_ibo.getBuffer()
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
