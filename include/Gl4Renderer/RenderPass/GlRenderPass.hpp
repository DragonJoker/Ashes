/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/RenderPass/RenderPass.hpp>

namespace gl_renderer
{
	struct AttachmentDescription
	{
		uint32_t index;
		std::reference_wrapper< VkAttachmentDescription const > attach;
	};

	class RenderPass
	{
	public:
		RenderPass( Device const & device
			, VkRenderPassCreateInfo createInfo );
		/**
		*\copydoc	RenderPass::createFrameBuffer
		*/
		ashes::FrameBufferPtr createFrameBuffer( VkExtent2D const & dimensions
			, ashes::FrameBufferAttachmentArray textures )const;

		uint32_t getAttachmentIndex( VkAttachmentDescription const & attach )const;

		inline AttachmentDescriptionArray const & getAttachments()const
		{
			return m_attachments;
		}

		inline std::vector< AttachmentDescription > const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

		inline bool hasDepthAttach()const
		{
			return m_hasDepthAttach;
		}

		inline VkAttachmentDescription const & getDepthAttach()const
		{
			return m_depthAttach;
		}

	private:
		Device const & m_device;
		bool m_hasDepthAttach{ false };
		VkAttachmentDescription m_depthAttach;
		std::vector< AttachmentDescription > m_colourAttaches;
		AttachmentDescriptionArray m_attachments;
	};
}
