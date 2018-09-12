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
	*	Commande d'�criture de timestamp.
	*/
	class WriteTimestampCommand
		: public CommandBase
	{
	public:
		WriteTimestampCommand( Device const & device
			, renderer::PipelineStageFlag pipelineStage
			, renderer::QueryPool const & pool
			, uint32_t query );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GLuint m_query;
	};
}
