/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	struct ClearAttachmentView
	{
		VkClearAttachment clear;
		ID3D11View * view;
	};

	using ClearAttachmentViewArray = std::vector< ClearAttachmentView >;

	class ClearAttachmentsCommand
		: public CommandBase
	{
	public:
		ClearAttachmentsCommand( VkDevice device
			, VkRenderPass renderPass
			, VkSubpassDescription const & subpass
			, VkFramebuffer framebuffer
			, VkClearAttachmentArray const & clearAttaches
			, VkClearRectArray const & clearRects );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkClearRectArray m_clearRects;
		ClearAttachmentViewArray m_clearViews;
	};
}
