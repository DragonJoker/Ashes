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
			, RenderSubpass const & subpass );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Device const & m_device;
		FrameBuffer const & m_frameBuffer;
		RenderSubpass const & m_subpass;
	};
}
