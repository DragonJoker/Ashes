/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class WriteTimestampCommand
		: public CommandBase
	{
	public:
		WriteTimestampCommand( VkDevice device
			, VkPipelineStageFlagBits pipelineStage
			, VkQueryPool pool
			, uint32_t query );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
