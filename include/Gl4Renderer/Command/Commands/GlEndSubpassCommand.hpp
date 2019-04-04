/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( VkDevice device
			, VkFramebuffer frameBuffer
			, VkSubpassDescription const & subpass );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkFramebuffer m_frameBuffer;
		VkSubpassDescription const & m_subpass;
	};
}
