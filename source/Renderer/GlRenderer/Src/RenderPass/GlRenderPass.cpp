/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/GlRenderPass.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Command/GlCommandBuffer.hpp"
#include "Image/GlTexture.hpp"

namespace gl_renderer
{
	RenderPass::RenderPass( renderer::Device const & device
		, renderer::RenderPassCreateInfo && createInfo )
		: renderer::RenderPass{ device, createInfo }
	{
		uint32_t index = 0u;

		for ( auto & attach : getAttachments() )
		{
			if ( renderer::isDepthOrStencilFormat( attach.format ) )
			{
				m_depthAttach = attach;
			}
			else
			{
				m_colourAttaches.push_back( { index++, std::ref( attach ) } );
			}
		}
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && textures )const
	{
		return std::make_unique< FrameBuffer >( *this
			, dimensions
			, std::move( textures ) );
	}

	uint32_t RenderPass::getAttachmentIndex( renderer::AttachmentDescription const & attach )const
	{
		uint32_t result = 0u;

		if ( !renderer::isDepthOrStencilFormat( attach.format ) )
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
