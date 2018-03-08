/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassAttachment.hpp>

namespace gl_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une sous passe de rendu.
	*\~english
	*\brief
	*	Describes a render subpass.
	*/
	class RenderSubpass
		: public renderer::RenderSubpass
	{
	public:
		RenderSubpass( renderer::Device const & device
			, renderer::PipelineBindPoint pipelineBindPoint
			, renderer::RenderSubpassState const & state
			, renderer::RenderSubpassAttachmentArray const & inputAttaches
			, renderer::RenderSubpassAttachmentArray const & colourAttaches
			, renderer::RenderSubpassAttachmentArray const & resolveAttaches
			, renderer::RenderSubpassAttachment const * depthAttach
			, renderer::UInt32Array const & preserveAttaches );
		void resolveAttachments( FrameBuffer const & fbo )const;

		inline renderer::RenderSubpassAttachmentArray const & getAttaches()const
		{
			return m_attaches;
		}

		inline bool hasResolveAttaches()const
		{
			return !m_resolveAttaches.empty();
		}

		inline uint32_t getResolveAttachesCount()const
		{
			return uint32_t( m_resolveAttaches.size() );
		}

	private:
		renderer::RenderSubpassAttachmentArray m_attaches;
		renderer::RenderSubpassAttachmentArray m_resolveAttaches;
	};
}
