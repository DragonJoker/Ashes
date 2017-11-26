/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

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
		DepthStencilState( DepthStencilStateFlags flags
			, bool depthTestEnable
			, bool depthWriteEnable
			, CompareOp depthCompareOp
			, bool depthBoundsTestEnable
			, bool stencilTestEnable
			, StencilOpState const & front
			, StencilOpState const & back
			, float minDepthBounds
			, float maxDepthBounds );
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
