/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/MultisampleState.hpp>

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Wrapper de vk::MultisampleState.
	*/
	class MultisampleState
	{
	public:
		MultisampleState( MultisampleStateFlags flags = 0
			, SampleCountFlag rasterizationSamples = SampleCountFlag::e1
			, bool sampleShadingEnable = false
			, float minSampleShading = 1.0f
			, bool alphaToCoverageEnable = false
			, bool alphaToOneEnable = false );
		MultisampleState( MultisampleStateFlags flags
			, SampleCountFlag rasterizationSamples
			, bool sampleShadingEnable
			, float minSampleShading
			, uint32_t sampleMask
			, bool alphaToCoverageEnable
			, bool alphaToOneEnable );
		/**
		*\return
		*	Le vk::MultisampleState.
		*/
		inline vk::MultisampleState const & getState()const
		{
			return m_state;
		}

	private:
		vk::MultisampleState m_state;
	};
}
