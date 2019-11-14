/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/D3D11RenderPass.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <algorithm>

namespace ashes::d3d11
{
	RenderPass::RenderPass( VkDevice device
		, VkRenderPassCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_attachments{ makeVector( createInfo.pAttachments, createInfo.attachmentCount ) }
		, m_subpasses{ makeVector( createInfo.pSubpasses, createInfo.subpassCount ) }
		, m_dependencies{ makeVector( createInfo.pDependencies, createInfo.dependencyCount ) }
	{
		m_createInfo.pAttachments = m_attachments.data();
		m_createInfo.pSubpasses = m_subpasses.data();
		m_createInfo.pDependencies = m_dependencies.data();

		for ( auto & subpass : m_subpasses )
		{
			auto data = std::make_unique< SubpassDescriptionData >( SubpassDescriptionData
				{
					makeVector( subpass.pInputAttachments, subpass.inputAttachmentCount ),
					makeVector( subpass.pColorAttachments, subpass.colorAttachmentCount ),
					makeVector( subpass.pResolveAttachments, subpass.colorAttachmentCount ),
					( subpass.pDepthStencilAttachment
						? Optional< VkAttachmentReference >( *subpass.pDepthStencilAttachment )
						: ashes::nullopt ),
					makeVector( subpass.pPreserveAttachments, subpass.preserveAttachmentCount )
				} );
			referenceAttaches( data->colorAttachments );
			referenceAttaches( data->inputAttachments );
			referenceAttaches( data->resolveAttachments );
			referenceAttaches( data->depthStencilAttachment );
			subpass.pColorAttachments = data->colorAttachments.data();
			subpass.pInputAttachments = data->inputAttachments.data();
			subpass.pResolveAttachments = data->resolveAttachments.data();
			subpass.pDepthStencilAttachment = ( bool( data->depthStencilAttachment )
				? &data->depthStencilAttachment.value()
				: nullptr );
			subpass.pPreserveAttachments = data->reserveAttachments.data();
			m_subpassInfos.emplace( &subpass
				, std::move( data ) );
		}
	}

	RenderPass::~RenderPass()
	{
	}

	VkAttachmentDescription const * RenderPass::findAttachment( uint32_t referenceIndex )const
	{
		if ( referenceIndex >= m_referencedAttachments.size() )
		{
			return nullptr;
		}

		return &getAttachment( m_referencedAttachments[referenceIndex] );
	}

	VkAttachmentDescription const & RenderPass::getAttachment( VkAttachmentReference const & reference )const
	{
		return m_attachments[reference.attachment];
	}

	void RenderPass::referenceAttaches( VkAttachmentReference const & value )
	{
		auto it = std::find_if( m_referencedAttachments.begin()
			, m_referencedAttachments.end()
			, [&value]( VkAttachmentReference const & lookup )
			{
				return value.attachment == lookup.attachment;
			} );

		if ( it == m_referencedAttachments.end() )
		{
			assert( value.attachment < m_attachments.size() && "Trying to reference a non existing attachment." );
			m_referencedAttachments.push_back( value );
		}
	}

	void RenderPass::referenceAttaches( Optional< VkAttachmentReference > const & value )
	{
		if ( bool( value ) )
		{
			referenceAttaches( value.value() );
		}
	}

	void RenderPass::referenceAttaches( VkAttachmentReferenceArray const & values )
	{
		for ( auto & value : values )
		{
			referenceAttaches( value );
		}
	}
}
