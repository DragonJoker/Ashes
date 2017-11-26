/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RenderPass.hpp"

#include "FrameBuffer.hpp"
#include "Device.hpp"
#include "CommandBuffer.hpp"
#include "RenderPassState.hpp"
#include "RenderSubpass.hpp"
#include "Texture.hpp"

namespace renderer
{
	namespace
	{
		std::vector< vk::RenderSubpass > doConvert( std::vector< RenderSubpass > const & subpasses )
		{
			std::vector< vk::RenderSubpass > result;

			for ( auto & subpass : subpasses )
			{
				result.emplace_back( subpass.getRenderSubpass() );
			}

			return result;
		}
	}

	RenderPass::RenderPass( Device const & device
		, std::vector< utils::PixelFormat > const & formats
		, std::vector< RenderSubpass > const & subpasses
		, RenderPassState const & initialState
		, RenderPassState const & finalState
		, bool clear
		, SampleCountFlag samplesCount )
		: m_renderPass( device.getDevice()
			, convert< VkFormat >( formats )
			, doConvert( subpasses )
			, initialState.getState()
			, finalState.getState()
			, clear
			, convert( samplesCount ) )
	{
	}

	RenderPass::~RenderPass()
	{
	}
}
