/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestPushConstantsCommand.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Shader/TestShaderModule.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	PushConstantsCommand::PushConstantsCommand( VkDevice device
		, PushConstantsBuffer const & pcb )
		: CommandBase{ device }
		, m_pcb{ pcb }
	{
	}

	PushConstantsCommand::PushConstantsCommand( VkDevice device
		, PushConstantsDesc const & )
		: CommandBase{ device }
	{
	}

	void PushConstantsCommand::apply()const
	{
	}

	CommandPtr PushConstantsCommand::clone()const
	{
		return std::make_unique< PushConstantsCommand >( *this );
	}
}
