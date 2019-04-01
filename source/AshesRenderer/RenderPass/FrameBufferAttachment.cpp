/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/RenderPass/FrameBufferAttachment.hpp"

namespace ashes
{
	FrameBufferAttachment::FrameBufferAttachment( AttachmentDescription const & attach
		, ImageViewPtr view )
		: m_frameBuffer{ nullptr }
		, m_view{ std::move( view ) }
		, m_attach{ attach }
	{
	}
}
