/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de démarrage d'une requête.
	*/
	class BeginQueryCommand
		: public CommandBase
	{
	public:
		BeginQueryCommand( renderer::QueryPool const & pool
			, uint32_t query
			, renderer::QueryControlFlags flags );
		void apply()const override;
		CommandPtr clone()const override;

	private:
		GLenum m_target;
		GLuint m_query;
	};
}
