/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class ExecuteCommandsCommand
		: public CommandBase
	{
	public:
		ExecuteCommandsCommand( VkDevice device
			, VkCommandBuffer commandBuffer );
		~ExecuteCommandsCommand();
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkCommandBuffer m_commandBuffer;
	};
}
