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
		uint32_t index = 0u;
		std::vector< uint32_t > indices;

		for ( auto itAttach = createInfo.pAttachments;
			itAttach != createInfo.pAttachments + createInfo.attachmentCount;
			++itAttach )
		{
			if ( isDepthOrStencilFormat( itAttach->format ) )
			{
				m_hasDepthAttach = true;
				m_depthAttach = *itAttach;
				indices.push_back( 0u );
			}
			else
			{
				indices.push_back( index );
				m_colourAttaches.push_back( { index, *itAttach } );
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
