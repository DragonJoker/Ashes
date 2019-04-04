/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class RenderPass
	{
	public:
		RenderPass( VkDevice device
			, VkRenderPassCreateInfo createInfo );

		uint32_t getAttachmentIndex( VkAttachmentDescription const & attach )const;

		inline VkAttachmentDescriptionArray const & getAttachments()const
		{
			return m_attachments;
		}

		inline VkSubpassDescriptionArray const & getSubpasses()const
		{
			return m_subpasses;
		}

		inline AttachmentDescriptionArray const & getColourAttaches()const
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

		inline VkExtent2D getRenderAreaGranularity()const
		{
			return VkExtent2D{ 1u, 1u };
		}

	private:
		VkDevice m_device;
		VkRenderPassCreateFlags m_flags;
		uint32_t m_attachmentCount;
		VkAttachmentDescriptionArray m_attachments;
		VkSubpassDescriptionArray m_subpasses;
		VkSubpassDependencyArray m_dependencies;
		bool m_hasDepthAttach{ false };
		VkAttachmentDescription m_depthAttach;
		AttachmentDescriptionArray m_colourAttaches;
	};
}
