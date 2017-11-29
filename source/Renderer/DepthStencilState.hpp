/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "StencilOpState.hpp"

#include <VkLib/DepthStencilState.hpp>

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Wrapper de VkDepthStencilState.
	*\~english
	*\brief
	*	VkDepthStencilState wrapper.
	*/
	class DepthStencilState
	{
	public:
		DepthStencilState( DepthStencilStateFlags flags = DepthStencilStateFlags{}
			, bool depthTestEnable = true
			, bool depthWriteEnable = true
			, CompareOp depthCompareOp = CompareOp::eLess
			, bool depthBoundsTestEnable = false
			, bool stencilTestEnable = false
			, StencilOpState const & front = StencilOpState{}
			, StencilOpState const & back = StencilOpState{}
			, float minDepthBounds = 0.0f
			, float maxDepthBounds = 1.0f );
		/**
		*\~french
		*\return
		*	Le VkPipelineDepthStencilStateCreateInfo engendré.
		*\~english
		*\return 
		*	The resulting VkPipelineDepthStencilStateCreateInfo.
		*/
		inline vk::DepthStencilState const & getState()const
		{
			return m_state;
		}

	private:
		vk::DepthStencilState m_state;
	};
}
