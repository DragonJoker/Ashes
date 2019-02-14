/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( Device const & device
			, ashes::FrameBuffer const & frameBuffer
			, ashes::SubpassDescription const & subpass );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		FrameBuffer const & m_frameBuffer;
		ashes::SubpassDescription const & m_subpass;
	};
}
