/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class ExecuteCommandsCommand
		: public CommandBase
	{
	public:
		ExecuteCommandsCommand( Device const & device
			, ashes::CommandBuffer const & commandBuffer );
		~ExecuteCommandsCommand();
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		CommandBuffer const & m_commandBuffer;
	};
}
