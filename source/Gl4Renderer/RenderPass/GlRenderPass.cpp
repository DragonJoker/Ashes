/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/GlRenderPass.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include <Ashes/RenderPass/RenderPassCreateInfo.hpp>

#include <algorithm>

namespace gl_renderer
{
	RenderPass::RenderPass( Device const & device
		, ashes::RenderPassCreateInfo createInfo )
		: ashes::RenderPass{ device, createInfo }
		, m_device{ device }
	{
		uint32_t index = 0u;
		std::vector< uint32_t > indices;

		for ( auto & attach : getAttachments() )
		{
			if ( ashes::isDepthOrStencilFormat( attach.format ) )
			{
				m_hasDepthAttach = true;
				m_depthAttach = attach;
				indices.push_back( 0u );
			}
			else
			{
				indices.push_back( index );
				m_colourAttaches.push_back( { index, std::ref( attach ) } );
				++index;
			}
		}

		for ( auto & subpass : getSubpasses() )
		{
			for ( auto & reference : subpass.colorAttachments )
			{
				reference.attachment = indices[reference.attachment];
			}

			for ( auto & reference : subpass.inputAttachments )
			{
				reference.attachment = indices[reference.attachment];
			}

			for ( auto & reference : subpass.resolveAttachments )
			{
				reference.attachment = indices[reference.attachment];
			}

			for ( auto & reference : subpass.resolveAttachments )
			{
				reference.attachment = indices[reference.attachment];
			}

			if ( bool( subpass.depthStencilAttachment ) )
			{
				auto & reference = subpass.depthStencilAttachment.value();
				reference.attachment = indices[reference.attachment];
			}
		}
	}

	ashes::FrameBufferPtr RenderPass::createFrameBuffer( ashes::Extent2D const & dimensions
		, ashes::FrameBufferAttachmentArray textures )const
	{
		return std::make_unique< FrameBuffer >( m_device
			, *this
			, dimensions
			, std::move( textures ) );
	}

	uint32_t RenderPass::getAttachmentIndex( ashes::AttachmentDescription const & attach )const
	{
		uint32_t result = 0u;

		if ( !ashes::isDepthOrStencilFormat( attach.format ) )
		{
			auto it = std::find_if( m_colourAttaches.begin()
				, m_colourAttaches.end()
				, [&attach]( AttachmentDescription const & lookup )
				{
					return &attach == &lookup.attach.get();
				} );
			assert( it != m_colourAttaches.end() );
			result = it->index;
		}

		return result;
	}
}
