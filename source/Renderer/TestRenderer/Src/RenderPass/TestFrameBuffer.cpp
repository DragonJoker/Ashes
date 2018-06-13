/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/TestFrameBuffer.hpp"

#include "Core/TestDevice.hpp"
#include "Image/TestTextureView.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include <RenderPass/FrameBufferAttachment.hpp>

namespace test_renderer
{
	namespace
	{
		TextureViewCRefArray convert( renderer::FrameBufferAttachmentArray const & attachs )
		{
			TextureViewCRefArray result;
			result.reserve( attachs.size() );

			for ( auto & attach : attachs )
			{
				result.emplace_back( static_cast< TextureView const & >( attach.getView() ) );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && attachments )
		: renderer::FrameBuffer{ renderPass, dimensions, std::move( attachments ) }
		, m_device{ device }
		, m_views{ convert( m_attachments ) }
		, m_dimensions{ dimensions }
	{
	}

	FrameBuffer::~FrameBuffer()
	{
	}
}
