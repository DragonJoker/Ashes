/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestEndSubpassCommand.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "RenderPass/TestFrameBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	EndSubpassCommand::EndSubpassCommand( VkDevice device
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass )
		: CommandBase{ device }
		, m_frameBuffer{ frameBuffer }
		, m_subpass{ subpass }
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
