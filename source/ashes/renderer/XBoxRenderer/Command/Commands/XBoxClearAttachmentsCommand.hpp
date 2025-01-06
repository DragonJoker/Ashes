/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	struct ClearAttachmentView
	{
		ClearAttachmentView( VkClearAttachment clear
			, ID3D11View * view )
			: clear{ std::move( clear ) }
			, view{ view }
		{
		}

		VkClearAttachment clear;
		ID3D11View * view;
	};

	using ClearAttachmentViewArray = std::vector< ClearAttachmentView >;

	class ClearAttachmentsCommand
		: public CommandBase
	{
	public:
		ClearAttachmentsCommand( VkDevice device
			, VkSubpassDescription const & subpass
			, VkFramebuffer framebuffer
			, ArrayView< VkClearAttachment const > const & clearAttaches
			, ArrayView< VkClearRect const > const & clearRects );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkClearRectArray m_clearRects;
		ClearAttachmentViewArray m_clearViews;
	};
}
