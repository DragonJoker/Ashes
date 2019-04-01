/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/RenderPass/FrameBuffer.hpp"
#include "AshesRenderer/RenderPass/FrameBufferAttachment.hpp"

#include "AshesRenderer/Command/Queue.hpp"
#include "AshesRenderer/Core/Device.hpp"
#include "AshesRenderer/Image/Image.hpp"
#include "AshesRenderer/RenderPass/RenderPass.hpp"

namespace ashes
{
	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, Extent2D const & dimensions
		, FrameBufferAttachmentArray attachments )
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
