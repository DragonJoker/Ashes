/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDispatchIndirectCommand.hpp"

namespace ashes::test
{
	DispatchIndirectCommand::DispatchIndirectCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize offset )
		: CommandBase{ device }
		, m_buffer{ buffer }
		, m_offset{ uint32_t( offset ) }
	{
	}

	void DispatchIndirectCommand::apply()const
	{
	}

	CommandPtr DispatchIndirectCommand::clone()const
	{
		return std::make_unique< DispatchIndirectCommand >( *this );
	}
}
