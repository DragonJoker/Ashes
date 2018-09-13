/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11CopyBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include <Miscellaneous/BufferCopy.hpp>

namespace d3d11_renderer
{
	CopyBufferCommand::CopyBufferCommand( Device const & device
		, renderer::BufferCopy const & copyInfo
		, renderer::BufferBase const & src
		, renderer::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_dstOffset{ copyInfo.dstOffset }
		, m_srcBox{ copyInfo.srcOffset, 0u, 0u, copyInfo.srcOffset + copyInfo.size, 1u, 1u }
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
