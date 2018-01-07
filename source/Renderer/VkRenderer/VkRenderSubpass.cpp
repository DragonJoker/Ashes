/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRenderSubpass.hpp"

#include "VkDevice.hpp"
#include "VkRenderSubpassState.hpp"

namespace vk_renderer
{
	RenderSubpass::RenderSubpass( Device const & device
		, std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassState const & neededState )
		: renderer::RenderSubpass{ device, formats, neededState }
		, m_device{ device }
		, m_neededState{ neededState }
	{
		// On cr�e les attaches pour les tampons de couleur et de profondeur.
		uint32_t index{ 0 };

		for ( auto const & format : formats )
		{
			if ( isDepthStencilFormat( convert( format ) ) )
			{
				m_depthReference.attachment = index;
			}
			else
			{
				m_colourReferences.push_back( { index, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
			}

			++index;
		}
	}

	VkSubpassDescription RenderSubpass::retrieveDescription()const
	{
		return
		{
			0u,                                                      // flags
			VK_PIPELINE_BIND_POINT_GRAPHICS,                         // pipelineBindPoint
			0u,                                                      // inputAttachmentCount
			nullptr,                                                 // pInputAttachments
			static_cast< uint32_t >( m_colourReferences.size() ),    // colorAttachmentCount
			m_colourReferences.data(),                               // pColorAttachments
			nullptr,                                                 // pResolveAttachments
			m_depthReference.attachment == 0xFFFFFFFF                // pDepthStencilAttachment
				? nullptr
				: &m_depthReference,
			0u,                                                      // preserveAttachmentCount
			nullptr,                                                 // pPreserveAttachments
		};
	}
}
