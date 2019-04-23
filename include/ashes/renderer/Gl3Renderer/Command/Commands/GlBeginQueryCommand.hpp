/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de d�marrage d'une requ�te.
	*/
	class BeginQueryCommand
		: public CommandBase
	{
	public:
		BeginQueryCommand( Device const & device
			, ashes::QueryPool const & pool
			, uint32_t query
			, ashes::QueryControlFlags flags );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GlQueryType m_target;
		GLuint m_query;
	};
}
