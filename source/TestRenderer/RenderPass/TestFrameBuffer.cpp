/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/TestFrameBuffer.hpp"

#include "Core/TestDevice.hpp"
#include "Image/TestImageView.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

namespace test_renderer
{
	namespace
	{
		ImageViewCRefArray convert( ashes::ashes::ImageViewPtrArray const & attachs )
		{
			ImageViewCRefArray result;
			result.reserve( attachs.size() );

			for ( auto & attach : attachs )
			{
				result.emplace_back( static_cast< ImageView const & >( attach.getView() ) );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, VkExtent2D const & dimensions
		, ashes::ashes::ImageViewPtrArray attachments )
		: ashes::FrameBuffer{ renderPass, dimensions, std::move( attachments ) }
		, m_device{ device }
		, m_views{ convert( m_attachments ) }
		, m_dimensions{ dimensions }
	{
	}

	FrameBuffer::~FrameBuffer()
	{
	}
}
