/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	class SetEventCommand
		: public CommandBase
	{
	public:
		SetEventCommand( renderer::Event const & event
			, renderer::PipelineStageFlags stageFlags );
		void apply()const override;
		CommandPtr clone()const override;

	private:
		renderer::Event const & m_event;
	};
}
