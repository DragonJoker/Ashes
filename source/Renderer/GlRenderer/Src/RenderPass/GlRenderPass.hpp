/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <RenderPass/RenderPass.hpp>

namespace gl_renderer
{
	struct AttachmentDescription
	{
		uint32_t index;
		std::reference_wrapper< renderer::AttachmentDescription const > attach;
	};

	class RenderPass
		: public renderer::RenderPass
	{
	public:
		RenderPass( renderer::Device const & device
			, renderer::RenderPassCreateInfo && createInfo );
		/**
		*\copydoc	renderer::RenderPass::createFrameBuffer
		*/
		renderer::FrameBufferPtr createFrameBuffer( renderer::Extent2D const & dimensions
			, renderer::FrameBufferAttachmentArray && textures )const override;

		uint32_t getAttachmentIndex( renderer::AttachmentDescription const & attach )const;

		inline std::vector< AttachmentDescription > const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

	private:
		renderer::AttachmentDescription m_depthAttach;
		std::vector< AttachmentDescription > m_colourAttaches;
	};
}
