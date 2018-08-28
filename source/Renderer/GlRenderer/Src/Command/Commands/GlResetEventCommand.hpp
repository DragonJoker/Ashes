/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de d�marrage d'une requ�te.
	*/
	class ResetEventCommand
		: public CommandBase
	{
	public:
		ResetEventCommand( Device const & device
			, renderer::Event const & event
			, renderer::PipelineStageFlags stageFlags );
		void apply()const override;
		CommandPtr clone()const override;

	private:
		renderer::Event const & m_event;
	};
}
