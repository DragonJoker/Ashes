/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RenderPass/RenderSubpass.hpp"

#include "Core/Device.hpp"
#include "RenderPass/RenderSubpassState.hpp"

namespace renderer
{
	RenderSubpass::RenderSubpass( Device const & device
		, PipelineBindPoint pipelineBindPoint
		, RenderSubpassState const & state
		, RenderSubpassAttachmentArray const & inputAttaches
		, RenderSubpassAttachmentArray const & colourAttaches
		, RenderSubpassAttachmentArray const & resolveAttaches
		, RenderSubpassAttachment const * depthAttach
		, UInt32Array const & preserveAttaches )
	{
	}
}
