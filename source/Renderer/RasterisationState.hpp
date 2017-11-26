/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/RasterisationState.hpp>

namespace renderer
{
	/**
	*\brief
	*	Wrapper de vk::RasterisationState.
	*/
	class RasterisationState
	{
	public:
		RasterisationState( RasterisationStateFlags flags = 0
			, bool depthClampEnable = false
			, bool rasterizerDiscardEnable = false
			, PolygonMode polygonMode = PolygonMode::eFill
			, CullModeFlags cullMode = CullModeFlag::eBack
			, FrontFace frontFace = FrontFace::eCounterClockwise
			, bool depthBiasEnable = false
			, float depthBiasConstantFactor = 0.0f
			, float depthBiasClamp = 0.0f
			, float depthBiasSlopeFactor = 0.0f
			, float lineWidth = 1.0f );
		/**
		*\return
		*	Le vk::RasterisationState.
		*/
		inline vk::RasterisationState const & getState()const
		{
			return m_state;
		}

	private:
		vk::RasterisationState m_state;
	};
}
