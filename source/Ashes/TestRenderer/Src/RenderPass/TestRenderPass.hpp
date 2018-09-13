/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <RenderPass/RenderPass.hpp>
#include <RenderPass/AttachmentDescription.hpp>
#include <RenderPass/RenderSubpassState.hpp>

namespace test_renderer
{
	class RenderPass
		: public ashes::RenderPass
	{
	public:
		RenderPass( Device const & device
			, ashes::RenderPassCreateInfo && createInfo );
		~RenderPass();
		/**
		*\copydoc	ashes::RenderPass::createFrameBuffer
		*/
		ashes::FrameBufferPtr createFrameBuffer( ashes::Extent2D const & dimensions
			, ashes::FrameBufferAttachmentArray && textures )const override;

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
