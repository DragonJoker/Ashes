/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxEndRenderPassCommand.hpp"

namespace ashes::xbox
{
	EndRenderPassCommand::EndRenderPassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_frameBuffer{ frameBuffer }
	{
	}

	void EndRenderPassCommand::apply( Context const & context )const
	{
		context.context->RSSetScissorRects( 0u, nullptr );
	}

	CommandPtr EndRenderPassCommand::clone()const
	{
		return std::make_unique< EndRenderPassCommand >( *this );
	}
}
