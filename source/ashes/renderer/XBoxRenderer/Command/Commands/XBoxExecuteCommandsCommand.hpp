/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class ExecuteCommandsCommand
		: public CommandBase
	{
	public:
		ExecuteCommandsCommand( VkDevice device
			, VkCommandBuffer commandBuffer );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkCommandBuffer m_commandBuffer;
	};
}
