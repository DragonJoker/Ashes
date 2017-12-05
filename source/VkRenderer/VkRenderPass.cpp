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
#include "VkTexture.hpp"

namespace vk_renderer
{
	namespace
	{
		std::vector< vk::RenderSubpass > doConvert( renderer::RenderSubpassPtrArray const & subpasses )
		{
			std::vector< vk::RenderSubpass > result;

			for ( auto & subpass : subpasses )
			{
				result.emplace_back( static_cast< RenderSubpass const & >( *subpass ).getRenderSubpass() );
			}

			return result;
		}
	}

	RenderPass::RenderPass( renderer::Device const & device
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
		, m_renderPass( static_cast< Device const & >( device ).getDevice()
			, convert< VkFormat >( formats )
			, doConvert( subpasses )
			, convert( initialState )
			, convert( finalState )
			, clear
			, convert( samplesCount ) )
	{
	}

	renderer::FrameBufferPtr RenderPass::createFrameBuffer( utils::IVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )const
	{
		return std::make_shared< FrameBuffer >( *this
			, dimensions
			, textures );
	}
}
