/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include "Shader/GlShaderDesc.hpp"

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
			, PushConstantsDesc const & pcb );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		PushConstantsDesc m_pcb;
	};
}
