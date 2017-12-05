/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkMultisampleState.hpp"

namespace vk_renderer
{
	vk::MultisampleState convert( renderer::MultisampleState const & state )
	{
		if ( state.getSampleMask() )
		{
			return vk::MultisampleState
			{
				convert( state.getFlags() ),
				convert( state.getRasterisationSamples() ),
				state.isSampleShadingEnabled(),
				state.getMinSampleShading(),
				state.getSampleMask(),
				state.isAlphaToCoverageEnabled(),
				state.isAlphaToOneEnabled()
			};
		}

		return vk::MultisampleState
		{
			convert( state.getFlags() ),
			convert( state.getRasterisationSamples() ),
			state.isSampleShadingEnabled(),
			state.getMinSampleShading(),
			state.isAlphaToCoverageEnabled(),
			state.isAlphaToOneEnabled()
		};
	}
}
