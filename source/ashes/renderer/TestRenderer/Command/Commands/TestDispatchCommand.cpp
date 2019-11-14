/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDispatchCommand.hpp"

namespace ashes::test
{
	DispatchCommand::DispatchCommand( VkDevice device
		, uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )
		: CommandBase{ device }
		, m_groupCountX{ groupCountX }
		, m_groupCountY{ groupCountY }
		, m_groupCountZ{ groupCountZ }
	{
	}

	void DispatchCommand::apply()const
	{
	}

	CommandPtr DispatchCommand::clone()const
	{
		return std::make_unique< DispatchCommand >( *this );
	}
}
