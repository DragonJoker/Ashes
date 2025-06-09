/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	class RenderPass
	{
	public:
		RenderPass( VkDevice device
			, VkRenderPassCreateInfo createInfo );

		VkAttachmentDescription const * findAttachment( uint32_t referenceIndex )const;
		VkAttachmentDescription const & getAttachment( VkAttachmentReference const & reference )const;

		VkSubpassDescriptionArray const & getSubpasses()const
		{
			return m_subpasses;
		}

		VkExtent2D getRenderAreaGranularity()const
		{
			return VkExtent2D{ 1u, 1u };
		}

		auto empty()const
		{
			return m_referencedAttachments.empty();
		}

		auto size()const
		{
			return m_referencedAttachments.size();
		}

		auto begin()
		{
			return m_referencedAttachments.begin();
		}

		auto end()
		{
			return m_referencedAttachments.end();
		}

		auto begin()const
		{
			return m_referencedAttachments.begin();
		}

		auto end()const
		{
			return m_referencedAttachments.end();
		}

		VkDevice getDevice()const
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
