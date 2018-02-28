/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>

namespace vk_renderer
{
	class RenderSubpass
		: public renderer::RenderSubpass
	{
	public:
		RenderSubpass( Device const & device
			, renderer::PipelineBindPoint pipelineBindPoint
			, renderer::RenderSubpassState const & state
			, renderer::RenderSubpassAttachmentArray const & inputAttaches
			, renderer::RenderSubpassAttachmentArray const & colourAttaches
			, renderer::RenderSubpassAttachmentArray const & resolveAttaches
			, renderer::RenderSubpassAttachment const * depthAttach
			, renderer::UInt32Array const & preserveAttaches );

		VkSubpassDescription const & retrieveDescription()const;

		inline renderer::RenderSubpassState const & getNeededState()const
		{
			return m_neededState;
		}

		inline operator VkSubpassDescription const &( )const
		{
			return m_description;
		}

	private:
		Device const & m_device;
		VkSubpassDescription m_description{};
		renderer::RenderSubpassState m_neededState{};
		std::vector< VkAttachmentReference > m_inputReferences;
		std::vector< VkAttachmentReference > m_colourReferences;
		std::vector< VkAttachmentReference > m_resolveReferences;
		renderer::UInt32Array m_preserveAttaches;
		VkAttachmentReference m_depthReference{ VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
	};
}
