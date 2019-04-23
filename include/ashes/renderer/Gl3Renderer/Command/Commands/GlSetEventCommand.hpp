/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	class SetEventCommand
		: public CommandBase
	{
	public:
		SetEventCommand( Device const & device
			, ashes::Event const & event
			, VkPipelineStageFlags stageFlags );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::Event const & m_event;
	};
}
