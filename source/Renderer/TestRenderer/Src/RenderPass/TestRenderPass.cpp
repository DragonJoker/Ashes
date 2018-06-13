/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/TestRenderPass.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"
#include "Image/TestTexture.hpp"
#include "Image/TestTextureView.hpp"
#include "RenderPass/TestFrameBuffer.hpp"

#include <RenderPass/RenderPassCreateInfo.hpp>
#include <RenderPass/RenderSubpassState.hpp>

#include <algorithm>

namespace test_renderer
{
	RenderPass::RenderPass( Device const & device
		, renderer::RenderPassCreateInfo && createInfo )
		: renderer::RenderPass{ device, createInfo }
		, m_device{ device }
	{
		m_subpassInfos.reserve( createInfo.subpasses.size() );

		for ( auto & subpass : createInfo.subpasses )
		{
			m_subpassInfos.push_back(
			{
				subpass.inputAttachments,
				subpass.colorAttachments,
				subpass.resolveAttachments,
				bool( subpass.depthStencilAttachment )
					? subpass.depthStencilAttachment.value()
					: renderer::AttachmentReference{ renderer::AttachmentUnused, renderer::ImageLayout::eUndefined }
			} );
		}
	}

	RenderPass::~RenderPass()
	{
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && attaches )const
	{
		return std::make_unique< FrameBuffer >( m_device
			, *this
			, dimensions
			, std::move( attaches ) );
	}
}
