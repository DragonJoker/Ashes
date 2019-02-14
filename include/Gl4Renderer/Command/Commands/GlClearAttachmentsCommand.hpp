/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/RenderPass/ClearAttachment.hpp>
#include <Ashes/RenderPass/ClearRect.hpp>

namespace gl_renderer
{
	class ClearAttachmentsCommand
		: public CommandBase
	{
	public:
		ClearAttachmentsCommand( Device const & device
			, ashes::ClearAttachmentArray const & clearAttaches
			, ashes::ClearRectArray const & clearRects );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::ClearAttachmentArray m_clearAttaches;
		ashes::ClearRectArray m_clearRects;
	};
}
