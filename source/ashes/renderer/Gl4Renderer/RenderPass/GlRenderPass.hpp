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

		VkAttachmentDescription const * findAttachment( uint32_t referenceIndex )const;
		VkAttachmentDescription const & getAttachment( VkAttachmentReference const & reference )const;

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

		inline VkExtent2D getRenderAreaGranularity()const
		{
			return VkExtent2D{ 1u, 1u };
		}

		inline auto empty()const
		{
			return m_referencedAttachments.empty();
		}

		inline auto size()const
		{
			return m_referencedAttachments.size();
		}

		inline auto begin()
		{
			return m_referencedAttachments.begin();
		}

		inline auto end()
		{
			return m_referencedAttachments.end();
		}

		inline auto begin()const
		{
			return m_referencedAttachments.begin();
		}

		inline auto end()const
		{
			return m_referencedAttachments.end();
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
		void referenceAttaches( VkAttachmentReference const & value );
		void referenceAttaches( Optional< VkAttachmentReference > const & value );
		void referenceAttaches( VkAttachmentReferenceArray const & value );

	private:
		VkDevice m_device;
		VkRenderPassCreateFlags m_flags;
		uint32_t m_attachmentCount;
		VkAttachmentDescriptionArray m_attachments;
		VkAttachmentReferenceArray m_referencedAttachments;
		VkSubpassDescriptionArray m_subpasses;
		SubpassDescriptionDataPtrMap m_subpassesDatas;
		VkSubpassDependencyArray m_dependencies;
		bool m_hasDepthAttach{ false };
		VkAttachmentDescriptionArray m_depthAttaches;
		AttachmentDescriptionArray m_colourAttaches;
	};
}
