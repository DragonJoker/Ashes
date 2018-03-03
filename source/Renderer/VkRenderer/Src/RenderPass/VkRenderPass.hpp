/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassAttachment.hpp>
#include <RenderPass/RenderSubpassState.hpp>

namespace vk_renderer
{
	class RenderPass
		: public renderer::RenderPass
	{
	public:
		RenderPass( Device const & device
			, renderer::RenderPassAttachmentArray const & attaches
			, renderer::RenderSubpassPtrArray && subpasses
			, renderer::RenderSubpassState const & initialState
			, renderer::RenderSubpassState const & finalState );
		~RenderPass();
		/**
		*\copydoc	renderer::RenderPass::createFrameBuffer
		*/
		renderer::FrameBufferPtr createFrameBuffer( renderer::UIVec2 const & dimensions
			, renderer::FrameBufferAttachmentArray && textures )const override;
		std::vector< VkClearValue > const & getClearValues( renderer::ClearValueArray const & clearValues )const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkRenderPass.
		*\~english
		*\brief
		*	VkRenderPass implicit cast operator.
		*/
		inline operator VkRenderPass const &( )const
		{
			return m_renderPass;
		}

	private:
		Device const & m_device;
		VkRenderPass m_renderPass{};
		RenderSubpassCRefArray m_subpasses;
		renderer::RenderSubpassState m_initialState;
		renderer::RenderSubpassState m_finalState;
		mutable std::vector< VkClearValue > m_clearValues;
	};
}
