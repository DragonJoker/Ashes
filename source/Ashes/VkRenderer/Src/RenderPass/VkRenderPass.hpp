/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <RenderPass/RenderPass.hpp>
#include <RenderPass/AttachmentDescription.hpp>
#include <RenderPass/RenderSubpassState.hpp>

namespace vk_renderer
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
		std::vector< VkClearValue > const & getClearValues( ashes::ClearValueArray const & clearValues )const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkRenderPass.
		*\~english
		*\brief
		*	VkRenderPass implicit cast operator.
		*/
		inline operator VkRenderPass const &( )const
		{
			return m_renderPass;
		}

	private:
		struct Subpass
		{
			std::vector< VkAttachmentReference > inputAttachments;
			std::vector< VkAttachmentReference > colorAttachments;
			std::vector< VkAttachmentReference > resolveAttachments;
			VkAttachmentReference depthStencilAttachment;
		};

	private:
		Device const & m_device;
		VkRenderPass m_renderPass{};
		std::vector< Subpass > m_subpassInfos;
		std::vector< VkSubpassDescription > m_subpasses;
		std::vector< VkSubpassDependency > m_dependencies;
		mutable std::vector< VkClearValue > m_clearValues;
	};
}
