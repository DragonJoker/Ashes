/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/AttachmentDescription.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>

namespace ashes::d3d11
{
	class RenderPass
		: public ashes::RenderPass
	{
	public:
		RenderPass( Device const & device
			, ashes::RenderPassCreateInfo createInfo );
		~RenderPass();
		/**
		*\copydoc	ashes::RenderPass::createFrameBuffer
		*/
		ashes::FrameBufferPtr createFrameBuffer( VkExtent2D const & dimensions
			, ashes::ImageViewPtrArray textures )const;

	private:
		struct Subpass
		{
			std::vector< ashes::AttachmentReference > inputAttachments;
			std::vector< ashes::AttachmentReference > colorAttachments;
			std::vector< ashes::AttachmentReference > resolveAttachments;
			ashes::AttachmentReference depthStencilAttachment;
		};

	private:
		Device const & m_device;
		std::vector< Subpass > m_subpassInfos;
	};
}
