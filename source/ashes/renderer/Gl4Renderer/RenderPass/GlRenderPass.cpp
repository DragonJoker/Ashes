/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/GlRenderPass.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

#include <algorithm>

namespace ashes
{
	inline VkAttachmentDescription deepCopy( VkAttachmentDescription const & rhs )
	{
		return rhs;
	}

	inline VkSubpassDescription deepCopy( VkSubpassDescription const & rhs )
	{
		return rhs;
	}

	inline VkSubpassDependency deepCopy( VkSubpassDependency const & rhs )
	{
		return rhs;
	}

	inline VkAttachmentReference deepCopy( VkAttachmentReference const & rhs )
	{
		return rhs;
	}

	inline uint32_t deepCopy( uint32_t const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
{
	RenderPass::RenderPass( VkDevice device
		, VkRenderPassCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_attachments{ makeVector( createInfo.pAttachments, createInfo.attachmentCount ) }
		, m_subpasses{ makeVector( createInfo.pSubpasses, createInfo.subpassCount ) }
		, m_dependencies{ makeVector( createInfo.pDependencies, createInfo.dependencyCount ) }
	{
		for ( auto & subpass : m_subpasses )
		{
			auto data = std::make_unique< SubpassDescriptionData >( SubpassDescriptionData
				{
					makeVector( subpass.pInputAttachments, subpass.inputAttachmentCount ),
					makeVector( subpass.pColorAttachments, subpass.colorAttachmentCount ),
					makeVector( subpass.pResolveAttachments, subpass.colorAttachmentCount ),
					( subpass.pDepthStencilAttachment
						? Optional< VkAttachmentReference >( *subpass.pDepthStencilAttachment )
						: std::nullopt ),
					makeVector( subpass.pPreserveAttachments, subpass.preserveAttachmentCount )
				} );
			subpass.pColorAttachments = data->colorAttachments.data();
			subpass.pInputAttachments = data->inputAttachments.data();
			subpass.pResolveAttachments = data->resolveAttachments.data();
			subpass.pDepthStencilAttachment = ( bool( data->depthStencilAttachment )
				? &data->depthStencilAttachment.value()
				: nullptr );
			subpass.pPreserveAttachments = data->reserveAttachments.data();
			m_subpassesDatas.emplace( &subpass
				, std::move( data ) );
		}

		uint32_t index = 0u;
		std::vector< uint32_t > indices;

		for ( auto & attach : m_attachments )
		{
			if ( isDepthOrStencilFormat( attach.format ) )
			{
				m_hasDepthAttach = true;
				m_depthAttach = attach;
				indices.push_back( 0u );
			}
			else
			{
				indices.push_back( index );
				m_colourAttaches.push_back( { index, attach } );
				++index;
			}
		}
	}

	uint32_t RenderPass::getAttachmentIndex( VkAttachmentDescription const & attach )const
	{
		uint32_t result = 0u;

		if ( !isDepthOrStencilFormat( attach.format ) )
		{
			auto it = std::find_if( m_colourAttaches.begin()
				, m_colourAttaches.end()
				, [&attach, &result]( AttachmentDescription const & lookup )
				{
					++result;
					return &attach == &lookup.attach.get();
				} );
			assert( it != m_colourAttaches.end() );
		}

		return result;
	}
}
