/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/D3D11RenderPass.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <Ashes/RenderPass/RenderPassCreateInfo.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>

#include <algorithm>

namespace d3d11_renderer
{
	RenderPass::RenderPass( Device const & device
		, ashes::RenderPassCreateInfo createInfo )
		: ashes::RenderPass{ device, createInfo }
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
					: ashes::AttachmentReference{ ashes::AttachmentUnused, ashes::ImageLayout::eUndefined }
			} );
		}
	}

	RenderPass::~RenderPass()
	{
	}

	ashes::FrameBufferPtr RenderPass::createFrameBuffer( ashes::Extent2D const & dimensions
		, ashes::FrameBufferAttachmentArray attaches )const
	{
		return std::make_unique< FrameBuffer >( m_device
			, *this
			, dimensions
			, std::move( attaches ) );
	}
}
