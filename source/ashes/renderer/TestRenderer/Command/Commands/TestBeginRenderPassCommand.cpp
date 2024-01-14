/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBeginRenderPassCommand.hpp"

#include "Buffer/TestBufferView.hpp"
#include "Core/TestDevice.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BeginRenderPassCommand::BeginRenderPassCommand( VkDevice device
		, VkRenderPass
		, VkFramebuffer
		, VkClearValueArray const & )
		: CommandBase{ device }
	{
	}

	void BeginRenderPassCommand::apply()const
	{
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
