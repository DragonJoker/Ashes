/*
This file belongs to Ashes.
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
			, ashes::Event const & event
			, ashes::PipelineStageFlags stageFlags );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::Event const & m_event;
	};
}
