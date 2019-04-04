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
	class BeginQueryCommand
		: public CommandBase
	{
	public:
		BeginQueryCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t query
			, VkQueryControlFlags flags );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		GlQueryType m_target;
		GLuint m_query;
	};
}
