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
	class PushConstantsCommand
		: public CommandBase
	{
	public:
		PushConstantsCommand( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::PushConstantsBufferBase const & pcb );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::PushConstantsBufferBase const & m_pcb;
		ashes::ByteArray m_data;
	};
}
