/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11DispatchIndirectCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

namespace d3d11_renderer
{
	DispatchIndirectCommand::DispatchIndirectCommand( Device const & device
		, ashes::BufferBase const & buffer
		, uint32_t offset )
		: CommandBase{ device }
		, m_buffer{ static_cast< Buffer const & >( buffer ) }
		, m_offset{ offset }
	{
	}

	void DispatchIndirectCommand::apply( Context const & context )const
	{
		context.context->DispatchIndirect( m_buffer.getBuffer(), m_offset );
	}

	CommandPtr DispatchIndirectCommand::clone()const
	{
		return std::make_unique< DispatchIndirectCommand >( *this );
	}
}
