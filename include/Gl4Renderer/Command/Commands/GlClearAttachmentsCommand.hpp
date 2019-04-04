/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class ClearAttachmentsCommand
		: public CommandBase
	{
	public:
		ClearAttachmentsCommand( VkDevice device
			, VkClearAttachmentArray clearAttaches
			, VkClearRectArray clearRects );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkClearAttachmentArray m_clearAttaches;
		VkClearRectArray m_clearRects;
	};
}
