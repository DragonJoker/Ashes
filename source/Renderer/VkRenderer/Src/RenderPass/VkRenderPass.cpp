/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RenderPass/VkRenderPass.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Core/VkDevice.hpp"
#include "Image/VkTexture.hpp"
#include "Image/VkTextureView.hpp"
#include "RenderPass/VkFrameBuffer.hpp"
#include "RenderPass/VkRenderSubpass.hpp"

#include <RenderPass/RenderSubpassState.hpp>

#include <algorithm>

namespace vk_renderer
{
	namespace
	{
		RenderSubpassCRefArray doConvert( renderer::RenderSubpassPtrArray const & subpasses )
		{
			RenderSubpassCRefArray result;

			for ( auto & subpass : subpasses )
			{
				result.emplace_back( static_cast< RenderSubpass const & >( *subpass ) );
			}

			return result;
		}
	}

	RenderPass::RenderPass( Device const & device
		, renderer::RenderPassAttachmentArray const & attaches
		, renderer::RenderSubpassPtrArray && subpasses
		, renderer::RenderSubpassState const & initialState
		, renderer::RenderSubpassState const & finalState )
		: renderer::RenderPass{ device
			, attaches
			, std::move( subpasses )
			, initialState
			, finalState }
		, m_device{ device }
		, m_subpasses{ doConvert( renderer::RenderPass::getSubpasses() ) }
		, m_initialState{ initialState }
		, m_finalState{ finalState }
	{
		// On crée les attaches pour les tampons de couleur et de profondeur.
		std::vector< VkAttachmentDescription > attachments;
		attachments.reserve( getSize() );

		for ( auto const & attach : *this )
		{
			attachments.push_back(
			{
				0u,                                             // flags
				convert( attach.format ),                       // format
				convert( attach.samples ),                      // samples
				convert( attach.loadOp ),                       // loadOp
				convert( attach.storeOp ),                      // storeOp
				convert( attach.stencilLoadOp ),                // stencilLoadOp
				convert( attach.stencilStoreOp ),               // stencilStoreOp
				convert( attach.initialLayout ),                // initialLayout
				convert( attach.finalLayout )                   // finalLayout
			} );
		}

		assert( !m_subpasses.empty() && "Can't create a render pass without subpass." );

		std::vector< VkSubpassDescription > descriptions;
		descriptions.reserve( m_subpasses.size() );
		std::vector< VkSubpassDependency > dependencies;
		dependencies.reserve( 1 + m_subpasses.size() );
		auto pipelineStage = convert( m_initialState.pipelineStage );
		auto access = convert( m_initialState.access );
		uint32_t subpassIndex = VK_SUBPASS_EXTERNAL;

		for ( auto const & subpass : m_subpasses )
		{
			auto const & state = subpass.get().getNeededState();

			if ( pipelineStage != convert( state.pipelineStage )
				|| access != convert( state.access ) )
			{
				dependencies.push_back(
				{
					subpassIndex,                                    // srcSubpass
					subpassIndex + 1,                                // dstSubpass
					pipelineStage,                                   // srcStageMask
					convert( state.pipelineStage ),                  // dstStageMask
					access,                                          // srcAccessMask
					convert( state.access ),                         // dstAccessMask
					VK_DEPENDENCY_BY_REGION_BIT                      // dependencyFlags
				} );
			}

			pipelineStage = convert( state.pipelineStage );
			access = convert( state.access );
			++subpassIndex;
			descriptions.push_back( subpass.get() );
		}

		if ( pipelineStage != convert( m_finalState.pipelineStage )
			|| access != convert( m_finalState.access ) )
		{
			dependencies.push_back(
			{
				subpassIndex,                                        // srcSubpass
				VK_SUBPASS_EXTERNAL,                                 // dstSubpass
				pipelineStage,                                       // srcStageMask
				convert( m_finalState.pipelineStage ),               // dstStageMask
				access,                                              // srcAccessMask
				convert( m_finalState.access ),                      // dstAccessMask
				VK_DEPENDENCY_BY_REGION_BIT                          // dependencyFlags
			} );
		}

		// Enfin on crée la passe
		VkRenderPassCreateInfo renderPassInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			nullptr,
			0u,                                                    // flags
			static_cast< uint32_t >( attachments.size() ),         // attachmentCount
			attachments.empty() ? nullptr : attachments.data(),    // pAttachments
			static_cast< uint32_t >( descriptions.size() ),        // subpassCount
			descriptions.empty() ? nullptr : descriptions.data(),  // pSubpasses
			static_cast< uint32_t >( dependencies.size() ),        // dependencyCount
			dependencies.empty() ? nullptr : dependencies.data(),  // pDependencies
		};
		DEBUG_DUMP( renderPassInfo );

		auto res = m_device.vkCreateRenderPass( m_device, &renderPassInfo, nullptr, &m_renderPass );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Render pass creation failed: " + getLastError() };
		}
	}

	RenderPass::~RenderPass()
	{
		m_device.vkDestroyRenderPass( m_device, m_renderPass, nullptr );
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( renderer::UIVec2 const & dimensions
		, renderer::FrameBufferAttachmentArray && attaches )const
	{
		return std::make_unique< FrameBuffer >( m_device
			, *this
			, dimensions
			, std::move( attaches ) );
	}

	std::vector< VkClearValue > const & RenderPass::getClearValues (renderer::ClearValueArray const & clearValues)const
	{
		m_clearValues = convert< VkClearValue >( clearValues );
		return m_clearValues;
	}
}
