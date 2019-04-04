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
	*	Commande de fin d'une passe de rendu, unbind son framebuffer.
	*/
	class ResetQueryPoolCommand
		: public CommandBase
	{
	public:
		ResetQueryPoolCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t firstQuery
			, uint32_t queryCount );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;
	};
}
