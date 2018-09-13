/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	class ExecuteCommandsCommand
		: public CommandBase
	{
	public:
		ExecuteCommandsCommand( Device const & device
			, renderer::CommandBuffer const & commandBuffer );
		~ExecuteCommandsCommand();
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		CommandBuffer const & m_commandBuffer;
	};
}
