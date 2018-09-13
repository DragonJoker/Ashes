/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/FrameBuffer.hpp"
#include "RenderPass/FrameBufferAttachment.hpp"

#include "Command/Queue.hpp"
#include "Core/Device.hpp"
#include "Image/Texture.hpp"
#include "RenderPass/RenderPass.hpp"

namespace ashes
{
	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, Extent2D const & dimensions
		, FrameBufferAttachmentArray && attachments )
		: m_device{ renderPass.getDevice() }
		, m_dimensions{ dimensions }
		, m_attachments{ std::move( attachments ) }
	{
		for ( auto & attachment : m_attachments )
		{
			attachment.m_frameBuffer = this;
		}

		registerObject( m_device, "FrameBuffer", this );
	}

	FrameBuffer::~FrameBuffer()
	{
		unregisterObject( m_device, this );
	}
}
