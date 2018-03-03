/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/GlRenderSubpass.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	RenderSubpass::RenderSubpass( renderer::Device const & device
		, renderer::PipelineBindPoint pipelineBindPoint
		, renderer::RenderSubpassState const & state
		, renderer::RenderSubpassAttachmentArray const & inputAttaches
		, renderer::RenderSubpassAttachmentArray const & colourAttaches
		, renderer::RenderSubpassAttachmentArray const & resolveAttaches
		, renderer::RenderSubpassAttachment const * depthAttach
		, renderer::UInt32Array const & preserveAttaches )
		: renderer::RenderSubpass{ device, pipelineBindPoint, state, inputAttaches, colourAttaches, resolveAttaches, depthAttach, preserveAttaches }
		, m_attaches{ colourAttaches }
	{
	}
}
