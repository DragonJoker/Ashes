/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestWriteTimestampCommand.hpp"

#include "Miscellaneous/TestQueryPool.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	WriteTimestampCommand::WriteTimestampCommand( VkDevice device
		, VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
	{
	}

	void WriteTimestampCommand::apply()const
	{
	}

	CommandPtr WriteTimestampCommand::clone()const
	{
		return std::make_unique< WriteTimestampCommand >( *this );
	}
}
