/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( VkDevice device
			, VkFramebuffer frameBuffer
			, VkSubpassDescription const & subpass );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkFramebuffer m_frameBuffer;
		VkSubpassDescription const & m_subpass;
		std::vector< ID3D11RenderTargetView * > m_attaches;
	};
}
