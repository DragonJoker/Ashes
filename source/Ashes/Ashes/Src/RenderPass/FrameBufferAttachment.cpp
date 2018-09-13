/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/FrameBufferAttachment.hpp"

namespace ashes
{
	FrameBufferAttachment::FrameBufferAttachment( AttachmentDescription const & attach
		, TextureView const & view )
		: m_frameBuffer{ nullptr }
		, m_view{ view }
		, m_attach{ attach }
	{
	}
}
