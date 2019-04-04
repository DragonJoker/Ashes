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
	*	Commande d'�criture de timestamp.
	*/
	class WriteTimestampCommand
		: public CommandBase
	{
	public:
		WriteTimestampCommand( VkDevice device
			, VkPipelineStageFlagBits pipelineStage
			, VkQueryPool pool
			, uint32_t query );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GLuint m_query;
	};
}
