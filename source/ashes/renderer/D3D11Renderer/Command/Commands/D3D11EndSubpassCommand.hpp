/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( VkDevice device
			, ashes::FrameBuffer const & frameBuffer
			, ashes::SubpassDescription const & subpass );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		FrameBuffer const & m_frameBuffer;
		ashes::SubpassDescription const & m_subpass;
		std::vector< ID3D11RenderTargetView * > m_attaches;
	};
}
