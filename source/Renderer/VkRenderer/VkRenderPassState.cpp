/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRenderPassState.hpp"

namespace vk_renderer
{
	namespace
	{
		std::vector< VkImageLayout > doConvert( renderer::ImageLayoutArray::const_iterator begin
			, renderer::ImageLayoutArray::const_iterator end )
		{
			std::vector< VkImageLayout > result;

			for ( auto it = begin; it != end; ++it )
			{
				result.push_back( convert( *it ) );
			}

			return result;
		}
	}

	vk::RenderPassState convert( renderer::RenderPassState const & state )
	{
		return vk::RenderPassState
		{
			convert( state.getPipelineStage() ),
			convert( state.getAccess() ),
			doConvert( state.begin(), state.end() )
		};
	}
}
