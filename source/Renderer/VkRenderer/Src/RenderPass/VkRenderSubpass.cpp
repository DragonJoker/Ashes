/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RenderPass/VkRenderSubpass.hpp"

#include "Core/VkDevice.hpp"

#include <RenderPass/RenderSubpassAttachment.hpp>

namespace vk_renderer
{
	RenderSubpass::RenderSubpass( Device const & device
		, renderer::PipelineBindPoint pipelineBindPoint
		, renderer::RenderSubpassState const & state
		, renderer::RenderSubpassAttachmentArray const & inputAttaches
		, renderer::RenderSubpassAttachmentArray const & colourAttaches
		, renderer::RenderSubpassAttachmentArray const & resolveAttaches
		, renderer::RenderSubpassAttachment const * depthAttach
		, renderer::UInt32Array const & preserveAttaches )
		: renderer::RenderSubpass{ device, pipelineBindPoint, state, inputAttaches, colourAttaches, resolveAttaches, depthAttach, preserveAttaches }
		, m_device{ device }
		, m_neededState{ state }
		, m_preserveAttaches{ preserveAttaches }
	{
		// On crée les attaches pour les tampons de couleur et de profondeur.
		for ( auto const & attach : inputAttaches )
		{
			m_inputReferences.push_back( { attach.attachment, convert( attach.layout ) } );
		}

		for ( auto const & attach : colourAttaches )
		{
			m_colourReferences.push_back( { attach.attachment, convert( attach.layout ) } );
		}

		for ( auto const & attach : resolveAttaches )
		{
			m_resolveReferences.push_back( { attach.attachment, convert( attach.layout ) } );
		}

		if ( depthAttach )
		{
			m_depthReference = { depthAttach->attachment, convert( depthAttach->layout ) };
		}

		m_description =
		{
			0u,                                                      // flags
			convert( pipelineBindPoint ),                            // pipelineBindPoint
			uint32_t( m_inputReferences.size() ),                    // inputAttachmentCount
			m_inputReferences.empty()                                // pInputAttachments
				? nullptr
				: m_inputReferences.data(),
			uint32_t( m_colourReferences.size() ),                   // colorAttachmentCount
			m_colourReferences.empty()                               // pColorAttachments
				? nullptr
				: m_colourReferences.data(),
			m_resolveReferences.empty()                              // pResolveAttachments
				? nullptr
				: m_resolveReferences.data(),
			depthAttach                                              // pDepthStencilAttachment
				? &m_depthReference
				: nullptr,
			uint32_t( m_preserveAttaches.size() ),                   // preserveAttachmentCount
			m_preserveAttaches.empty()                               // pPreserveAttachments
				? nullptr
				: m_preserveAttaches.data()
		};
	}

	VkSubpassDescription const & RenderSubpass::retrieveDescription()const
	{
		return m_description;
	}
}
