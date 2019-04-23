/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

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

	public:
		struct SubpassDescriptionData
		{
			VkAttachmentReferenceArray inputAttachments;
			VkAttachmentReferenceArray colorAttachments;
			VkAttachmentReferenceArray resolveAttachments;
			Optional< VkAttachmentReference > depthStencilAttachment;
			UInt32Array reserveAttachments;
		};
		using SubpassDescriptionDataPtr = std::unique_ptr< SubpassDescriptionData >;
		using SubpassDescriptionDataPtrMap = std::map< VkSubpassDescription const *, SubpassDescriptionDataPtr >;

	private:
		VkDevice m_device;
		VkRenderPassCreateFlags m_flags;
		uint32_t m_attachmentCount;
		VkAttachmentDescriptionArray m_attachments;
		VkSubpassDescriptionArray m_subpasses;
		SubpassDescriptionDataPtrMap m_subpassesDatas;
		VkSubpassDependencyArray m_dependencies;
		bool m_hasDepthAttach{ false };
		VkAttachmentDescription m_depthAttach;
		AttachmentDescriptionArray m_colourAttaches;
	};
}
