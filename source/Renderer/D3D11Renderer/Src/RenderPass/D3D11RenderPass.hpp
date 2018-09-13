/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <RenderPass/RenderPass.hpp>
#include <RenderPass/AttachmentDescription.hpp>
#include <RenderPass/RenderSubpassState.hpp>

namespace d3d11_renderer
{
	class RenderPass
		: public renderer::RenderPass
	{
	public:
		RenderPass( Device const & device
			, renderer::RenderPassCreateInfo && createInfo );
		~RenderPass();
		/**
		*\copydoc	renderer::RenderPass::createFrameBuffer
		*/
		renderer::FrameBufferPtr createFrameBuffer( renderer::Extent2D const & dimensions
			, renderer::FrameBufferAttachmentArray && textures )const override;

	private:
		struct Subpass
		{
			std::vector< renderer::AttachmentReference > inputAttachments;
			std::vector< renderer::AttachmentReference > colorAttachments;
			std::vector< renderer::AttachmentReference > resolveAttachments;
			renderer::AttachmentReference depthStencilAttachment;
		};

	private:
		Device const & m_device;
		std::vector< Subpass > m_subpassInfos;
	};
}
