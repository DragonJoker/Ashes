/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBeginSubpassCommand.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Image/TestImageView.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BeginSubpassCommand::BeginSubpassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_subpass{ subpass }
		, m_frameBuffer{ frameBuffer }
	{
	}

	void BeginSubpassCommand::apply()const
	{
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
