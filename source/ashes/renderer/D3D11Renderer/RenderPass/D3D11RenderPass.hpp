/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class RenderPass
	{
	public:
		RenderPass( VkDevice device
			, VkRenderPassCreateInfo createInfo );
		~RenderPass();

		uint32_t getAttachmentIndex( VkAttachmentDescription const & attach )const;

		inline VkAttachmentDescriptionArray const & getAttachments()const
		{
			return m_attachments;
		}

		inline VkSubpassDescriptionArray const & getSubpasses()const
		{
			return m_subpasses;
		}

		inline VkExtent2D getRenderAreaGranularity()const
		{
			return VkExtent2D{ 1u, 1u };
		}

	private:
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
		VkRenderPassCreateInfo m_createInfo;
		VkAttachmentDescriptionArray m_attachments;
		VkSubpassDescriptionArray m_subpasses;
		VkSubpassDependencyArray m_dependencies;
		SubpassDescriptionDataPtrMap m_subpassInfos;
	};
}
