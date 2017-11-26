/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "MultisampleState.hpp"

namespace renderer
{
	MultisampleState::MultisampleState( MultisampleStateFlags flags
		, SampleCountFlag rasterizationSamples
		, bool sampleShadingEnable
		, float minSampleShading
		, bool alphaToCoverageEnable
		, bool alphaToOneEnable )
		: m_state
		{
			convert( flags ),
			convert( rasterizationSamples ),
			sampleShadingEnable,
			minSampleShading,
			alphaToCoverageEnable,
			alphaToOneEnable
		}
	{
	}

	MultisampleState::MultisampleState( MultisampleStateFlags flags
		, SampleCountFlag rasterizationSamples
		, bool sampleShadingEnable
		, float minSampleShading
		, uint32_t sampleMask
		, bool alphaToCoverageEnable
		, bool alphaToOneEnable )
		: m_state
		{
			convert( flags ),
			convert( rasterizationSamples ),
			sampleShadingEnable,
			minSampleShading,
			sampleMask,
			alphaToCoverageEnable,
			alphaToOneEnable
		}
	{
	}
}
