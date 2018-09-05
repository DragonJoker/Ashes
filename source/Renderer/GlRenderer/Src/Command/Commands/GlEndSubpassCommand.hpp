/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( Device const & device
			, renderer::FrameBuffer const & frameBuffer
			, renderer::SubpassDescription const & subpass );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		FrameBuffer const & m_frameBuffer;
		renderer::SubpassDescription const & m_subpass;
	};
}
