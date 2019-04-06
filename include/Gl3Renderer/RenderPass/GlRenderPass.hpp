/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/RenderPass/RenderPass.hpp>

namespace gl_renderer
{
	struct AttachmentDescription
	{
		uint32_t index;
		std::reference_wrapper< ashes::AttachmentDescription const > attach;
	};

	class RenderPass
		: public ashes::RenderPass
	{
	public:
		RenderPass( Device const & device
			, ashes::RenderPassCreateInfo createInfo );
		/**
		*\copydoc	ashes::RenderPass::createFrameBuffer
		*/
		ashes::FrameBufferPtr createFrameBuffer( VkExtent2D const & dimensions
			, ashes::ashes::ImageViewPtrArray textures )const override;

		uint32_t getAttachmentIndex( ashes::AttachmentDescription const & attach )const;

		inline std::vector< AttachmentDescription > const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

		inline bool hasDepthAttach()const
		{
			return m_hasDepthAttach;
		}

		inline ashes::AttachmentDescription const & getDepthAttach()const
		{
			return m_depthAttach;
		}

	private:
		Device const & m_device;
		bool m_hasDepthAttach{ false };
		ashes::AttachmentDescription m_depthAttach;
		std::vector< AttachmentDescription > m_colourAttaches;
	};
}
