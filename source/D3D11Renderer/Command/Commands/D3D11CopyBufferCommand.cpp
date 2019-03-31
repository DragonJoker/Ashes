/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include <Ashes/Miscellaneous/BufferCopy.hpp>

namespace d3d11_renderer
{
	CopyBufferCommand::CopyBufferCommand( Device const & device
		, ashes::BufferCopy const & copyInfo
		, ashes::BufferBase const & src
		, ashes::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_dstOffset{ UINT( copyInfo.dstOffset ) }
		, m_srcBox{ UINT( copyInfo.srcOffset ), 0u, 0u, UINT( copyInfo.srcOffset + copyInfo.size ), 1u, 1u }
	{
	}

	void CopyBufferCommand::apply( Context const & context )const
	{
		context.context->CopySubresourceRegion( m_dst.getBuffer()
			, 0u
			, m_dstOffset
			, 0u
			, 0u
			, m_src.getBuffer()
			, 0u
			, &m_srcBox );
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
