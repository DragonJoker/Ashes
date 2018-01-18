/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlRenderPass.hpp"

#include "GlFrameBuffer.hpp"
#include "GlDevice.hpp"
#include "GlCommandBuffer.hpp"
#include "GlRenderSubpass.hpp"
#include "GlTexture.hpp"

namespace gl_renderer
{
	RenderPass::RenderPass( renderer::Device const & device
		, std::vector< renderer::PixelFormat > const & formats
		, renderer::RenderSubpassPtrArray const & subpasses
		, renderer::RenderPassState const & initialState
		, renderer::RenderPassState const & finalState
		, bool clear
		, renderer::SampleCountFlag samplesCount )
		: renderer::RenderPass{ device
			, formats
			, subpasses
			, initialState
			, finalState
			, clear
			, samplesCount }
		, m_clear{ clear }
	{
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( renderer::UIVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )const
	{
		return std::make_shared< FrameBuffer >( *this
			, dimensions
			, textures );
	}
}
