/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Commande de d�marrage d'une requ�te.
	*/
	class ResetEventCommand
		: public CommandBase
	{
	public:
		ResetEventCommand( VkDevice device
			, VkEvent event
			, VkPipelineStageFlags stageFlags );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkEvent m_event;
	};
}
