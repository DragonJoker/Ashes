/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/GlRenderPass.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Command/GlCommandBuffer.hpp"
#include "RenderPass/GlRenderSubpass.hpp"
#include "Image/GlTexture.hpp"

namespace gl_renderer
{
	namespace
	{
		RenderSubpassCRefArray doConvert( renderer::RenderSubpassPtrArray const & subpasses )
		{
			RenderSubpassCRefArray result;

			for ( auto & subpass : subpasses )
			{
				result.emplace_back( static_cast< RenderSubpass const & >( *subpass ) );
			}

			return result;
		}
	}

	RenderPass::RenderPass( renderer::Device const & device
		, renderer::RenderPassAttachmentArray const & attaches
		, renderer::RenderSubpassPtrArray && subpasses
		, renderer::RenderSubpassState const & initialState
		, renderer::RenderSubpassState const & finalState )
		: renderer::RenderPass{ device
			, attaches
			, std::move( subpasses )
			, initialState
			, finalState }
		, m_subpasses{ doConvert( renderer::RenderPass::getSubpasses() ) }
	{
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && textures )const
	{
		return std::make_unique< FrameBuffer >( *this
			, dimensions
			, std::move( textures ) );
	}
}
