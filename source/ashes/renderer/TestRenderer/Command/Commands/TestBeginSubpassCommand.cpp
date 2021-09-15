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
		, VkRenderPass
		, VkFramebuffer
		, VkSubpassDescription const & )
		: CommandBase{ device }
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
