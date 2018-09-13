/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( Device const & device
			, renderer::FrameBuffer const & frameBuffer
			, renderer::SubpassDescription const & subpass );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		FrameBuffer const & m_frameBuffer;
		renderer::SubpassDescription const & m_subpass;
		std::vector< ID3D11RenderTargetView * > m_attaches;
	};
}
