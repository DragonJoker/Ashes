/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRenderPass.hpp"

#include "VkFrameBuffer.hpp"
#include "VkDevice.hpp"
#include "VkCommandBuffer.hpp"
#include "VkRenderPassState.hpp"
#include "VkRenderSubpass.hpp"
#include "VkRenderSubpassState.hpp"
#include "VkSampleCountFlag.hpp"
#include "VkTexture.hpp"

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
		, std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassPtrArray const & subpasses
		, renderer::RenderPassState const & initialState
		, renderer::RenderPassState const & finalState
		, bool clear
		, renderer::SampleCountFlag samplesCount )
		: renderer::RenderPass{ device
			, formats
			, subpasses
			, initialState
			, finalState
			, clear
			, samplesCount }
		, m_device{ device }
		, m_formats{ formats }
		, m_subpasses{ doConvert( subpasses ) }
		, m_samplesCount{ samplesCount }
		, m_initialState{ initialState }
		, m_finalState{ finalState }
	{
		// On cr�e les attaches pour les tampons de couleur et de profondeur.
		std::vector< VkAttachmentDescription > attachments;
		attachments.reserve( m_formats.size() );
		uint32_t index{ 0 };

		for ( auto const & format : m_formats )
		{
			if ( isDepthStencilFormat( convert( format ) ) )
			{
				attachments.push_back(
				{
					0u,                                             // flags
					convert( format ),                              // format
					convert( m_samplesCount ),                      // samples
					clear                                           // loadOp
						? VK_ATTACHMENT_LOAD_OP_CLEAR
						: VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_STORE,                   // storeOp
					clear                                           // stencilLoadOp
						? VK_ATTACHMENT_LOAD_OP_CLEAR
						: VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_STORE,                   // stencilStoreOp
					convert( *( m_initialState.begin() + index ) ), // initialLayout
					convert( *( m_finalState.begin() + index ) )    // finalLayout
				} );
			}
			else
			{
				attachments.push_back(
				{
					0u,                                             // flags
					convert( format ),                              // format
					convert( m_samplesCount ),                      // samples
					clear                                           // loadOp
						? VK_ATTACHMENT_LOAD_OP_CLEAR
						: VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_STORE,                   // storeOp
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,                // stencilLoadOp
					VK_ATTACHMENT_STORE_OP_DONT_CARE,               // stencilStoreOp
					convert( *( m_initialState.begin() + index ) ), // initialLayout
					convert( *( m_finalState.begin() + index ) )    // finalLayout
				} );
			}

			++index;
		}

		assert( !m_subpasses.empty() && "Can't create a render pass without subpass." );

		std::vector< VkSubpassDescription > descriptions;
		descriptions.reserve( m_subpasses.size() );
		std::vector< VkSubpassDependency > dependencies;
		dependencies.reserve( 1 + m_subpasses.size() );
		RenderSubpassState currentState
		{
			convert( m_initialState.getPipelineStage() ),
			convert( m_initialState.getAccess() )
		};
		uint32_t subpassIndex = VK_SUBPASS_EXTERNAL;

		for ( auto const & subpass : m_subpasses )
		{
			auto const & state = subpass.get().getNeededState();

			if ( currentState.m_pipelineStage != state.getPipelineStage()
				|| currentState.m_access != state.getAccess() )
			{
				dependencies.push_back(
				{
					subpassIndex,                                    // srcSubpass
					subpassIndex + 1,                                // dstSubpass
					currentState.m_pipelineStage,                    // srcStageMask
					convert( state.getPipelineStage() ),             // dstStageMask
					currentState.m_access,                           // srcAccessMask
					convert( state.getAccess() ),                    // dstAccessMask
					VK_DEPENDENCY_BY_REGION_BIT                      // dependencyFlags
				} );
			}

			currentState = convert( state );
			++subpassIndex;
			descriptions.push_back( subpass.get() );
		}

		if ( currentState.m_pipelineStage != m_finalState.getPipelineStage()
			|| currentState.m_access != m_finalState.getAccess() )
		{
			dependencies.push_back(
			{
				subpassIndex,                                        // srcSubpass
				VK_SUBPASS_EXTERNAL,                                 // dstSubpass
				currentState.m_pipelineStage,                        // srcStageMask
				convert( m_finalState.getPipelineStage() ),          // dstStageMask
				currentState.m_access,                               // srcAccessMask
				convert( m_finalState.getAccess() ),                 // dstAccessMask
				VK_DEPENDENCY_BY_REGION_BIT                          // dependencyFlags
			} );
		}

		// Enfin on cr�e la passe
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

		auto res = CreateRenderPass( m_device, &renderPassInfo, nullptr, &m_renderPass );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Render pass creation failed: " + getLastError() };
		}
	}

	RenderPass::~RenderPass()
	{
		DestroyRenderPass( m_device, m_renderPass, nullptr );
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( utils::IVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )const
	{
		// On v�rifie la compatibilit� des vues demand�s pour le framebuffer � cr�er.
		auto it = std::find_if( textures.begin()
			, textures.end()
			, [this]( std::reference_wrapper< renderer::Texture const > const & view )
		{
			return m_formats.end() == std::find_if( m_formats.begin()
				, m_formats.end()
				, [&view]( auto format )
			{
				return format == view.get().getFormat();
			} );
		} );

		if ( it != textures.end() )
		{
			throw std::runtime_error{ "Incompatible views for framebuffer creation from this render pass." };
		}

		// On cr�e le framebuffer.
		return std::make_unique< FrameBuffer >( m_device
			, *this
			, dimensions
			, textures );
	}

	std::vector< VkClearValue > const & RenderPass::getClearValues (renderer::ClearValueArray const & clearValues)const
	{
		m_clearValues = convert< VkClearValue >( clearValues );
		return m_clearValues;
	}
}
