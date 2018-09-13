/*
This file belongs to Ashes.
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
			, ashes::FrameBuffer const & frameBuffer
			, ashes::SubpassDescription const & subpass );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		FrameBuffer const & m_frameBuffer;
		ashes::SubpassDescription const & m_subpass;
		std::vector< ID3D11RenderTargetView * > m_attaches;
	};
}
