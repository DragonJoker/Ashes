/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ExecuteCommandsCommand
		: public CommandBase
	{
	public:
		ExecuteCommandsCommand( VkDevice device
			, VkCommandBuffer commandBuffer );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		VkCommandBuffer m_commandBuffer;
	};
}
