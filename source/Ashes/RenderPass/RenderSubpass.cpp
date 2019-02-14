/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/RenderPass/RenderSubpass.hpp"

#include "Ashes/Core/Device.hpp"
#include "Ashes/RenderPass/RenderSubpassState.hpp"

namespace ashes
{
	RenderSubpass::RenderSubpass( PipelineBindPoint pipelineBindPoint
		, RenderSubpassState const & state
		, AttachmentReferenceArray const & inputAttaches
		, AttachmentReferenceArray const & colourAttaches
		, AttachmentReferenceArray const & resolveAttaches
		, AttachmentReference const * depthAttach
		, UInt32Array const & reserveAttaches )
		: m_description{
			0u,
			pipelineBindPoint,
			inputAttaches,
			colourAttaches,
			resolveAttaches,
			nullopt,
			reserveAttaches
		}
		, m_neededState{ state }
	{
		if ( depthAttach )
		{
			m_description.depthStencilAttachment = *depthAttach;
		}
	}

	RenderSubpass::RenderSubpass( PipelineBindPoint pipelineBindPoint
		, RenderSubpassState const & state
		, AttachmentReferenceArray const & colourAttaches )
		: RenderSubpass( pipelineBindPoint
			, state
			, AttachmentReferenceArray{}
			, colourAttaches
			, AttachmentReferenceArray{}
			, nullptr
			, UInt32Array{} )
	{
	}

	RenderSubpass::RenderSubpass( PipelineBindPoint pipelineBindPoint
		, RenderSubpassState const & state
		, AttachmentReferenceArray const & colourAttaches
		, AttachmentReference const & depthAttach )
		: RenderSubpass( pipelineBindPoint
			, state
			, AttachmentReferenceArray{}
			, colourAttaches
			, AttachmentReferenceArray{}
			, &depthAttach
			, UInt32Array{} )
	{
	}
}
