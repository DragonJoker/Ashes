/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( VkDevice device
			, VkFramebuffer frameBuffer
			, VkSubpassDescription const & subpass );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkFramebuffer m_frameBuffer;
		VkSubpassDescription const & m_subpass;
		std::vector< ID3D11RenderTargetView * > m_attaches;
	};
}
