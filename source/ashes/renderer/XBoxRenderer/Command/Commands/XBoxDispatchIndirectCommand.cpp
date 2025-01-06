/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxDispatchIndirectCommand.hpp"

#include "Buffer/XBoxBuffer.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	DispatchIndirectCommand::DispatchIndirectCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize offset )
		: CommandBase{ device }
		, m_buffer{ buffer }
		, m_offset{ UINT( offset ) }
	{
	}

	void DispatchIndirectCommand::apply( Context const & context )const
	{
		context.context->DispatchIndirect( get( m_buffer )->getBuffer(), m_offset );
	}

	CommandPtr DispatchIndirectCommand::clone()const
	{
		return std::make_unique< DispatchIndirectCommand >( *this );
	}
}
