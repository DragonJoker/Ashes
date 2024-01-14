/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class RenderPass
	{
	public:
		RenderPass( VkDevice device
			, VkRenderPassCreateInfo createInfo );

		VkAttachmentDescription const * findAttachment( uint32_t referenceIndex )const;
		VkAttachmentDescription const & getAttachment( VkAttachmentReference const & reference )const;

		VkSubpassDescriptionArray const & getSubpasses()const noexcept
		{
			return m_subpasses;
		}

		VkExtent2D getRenderAreaGranularity()const noexcept
		{
			return VkExtent2D{ 1u, 1u };
		}

		auto empty()const noexcept
		{
			return m_referencedAttachments.empty();
		}

		auto size()const noexcept
		{
			return m_referencedAttachments.size();
		}

		auto begin()noexcept
		{
			return m_referencedAttachments.begin();
		}

		auto end()noexcept
		{
			return m_referencedAttachments.end();
		}

		auto begin()const noexcept
		{
			return m_referencedAttachments.begin();
		}

		auto end()const noexcept
		{
			return m_referencedAttachments.end();
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
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
		void referenceAttaches( VkAttachmentReference const & value );
		void referenceAttaches( Optional< VkAttachmentReference > const & value );
		void referenceAttaches( VkAttachmentReferenceArray const & value );

	private:
		VkDevice m_device;
		VkRenderPassCreateInfo m_createInfo;
		VkAttachmentDescriptionArray m_attachments;
		VkAttachmentReferenceArray m_referencedAttachments;
		VkSubpassDescriptionArray m_subpasses;
		VkSubpassDependencyArray m_dependencies;
		SubpassDescriptionDataPtrMap m_subpassInfos;
	};
}
