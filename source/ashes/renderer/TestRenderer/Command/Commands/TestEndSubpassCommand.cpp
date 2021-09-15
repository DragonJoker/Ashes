/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestEndSubpassCommand.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "RenderPass/TestFrameBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	EndSubpassCommand::EndSubpassCommand( VkDevice device
		, VkFramebuffer
		, VkSubpassDescription const & )
		: CommandBase{ device }
	{
	}

	void EndSubpassCommand::apply()const
	{
	}

	CommandPtr EndSubpassCommand::clone()const
	{
		return std::make_unique< EndSubpassCommand >( *this );
	}
}
