/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de fin d'une requ�te.
	*/
	class EndQueryCommand
		: public CommandBase
	{
	public:
		EndQueryCommand( Device const & device
			, ashes::QueryPool const & pool
			, uint32_t query );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GlQueryType m_target;
	};
}
