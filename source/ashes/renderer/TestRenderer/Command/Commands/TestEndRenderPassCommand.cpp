/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestEndRenderPassCommand.hpp"

namespace ashes::test
{
	EndRenderPassCommand::EndRenderPassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_frameBuffer{ frameBuffer }
	{
	}

	void EndRenderPassCommand::apply()const
	{
	}

	CommandPtr EndRenderPassCommand::clone()const
	{
		return std::make_unique< EndRenderPassCommand >( *this );
	}
}
