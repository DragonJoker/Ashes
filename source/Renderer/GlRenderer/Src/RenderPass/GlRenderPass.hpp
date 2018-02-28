/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRenderSubpass.hpp"

#include <RenderPass/RenderPass.hpp>

namespace gl_renderer
{
	class RenderPass
		: public renderer::RenderPass
	{
	public:
		RenderPass( renderer::Device const & device
			, renderer::RenderPassAttachmentArray const & attaches
			, renderer::RenderSubpassPtrArray && subpasses
			, renderer::RenderSubpassState const & initialState
			, renderer::RenderSubpassState const & finalState );
		/**
		*\copydoc	renderer::RenderPass::createFrameBuffer
		*/
		renderer::FrameBufferPtr createFrameBuffer( renderer::UIVec2 const & dimensions
			, renderer::FrameBufferAttachmentArray && textures )const override;

		inline RenderSubpassCRefArray const & getSubpasses()const
		{
			return m_subpasses;
		}

	private:
		RenderSubpassCRefArray m_subpasses;
	};
}
