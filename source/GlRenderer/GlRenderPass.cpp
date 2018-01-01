/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlRenderPass.hpp"

#include "GlFrameBuffer.hpp"
#include "GlDevice.hpp"
#include "GlCommandBuffer.hpp"
#include "GlRenderPassState.hpp"
#include "GlRenderSubpass.hpp"
#include "GlTexture.hpp"

namespace gl_renderer
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
