/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Wrapper de VkStencilOpState.
	*/
	class StencilOpState
	{
	public:
		StencilOpState( StencilOp failOp = StencilOp::eKeep
			, StencilOp passOp = StencilOp::eKeep
			, StencilOp depthFailOp = StencilOp::eKeep
			, CompareOp compareOp = CompareOp::eAlways
			, uint32_t compareMask = 0xFFFFFFFFu
			, uint32_t writeMask = 0xFFFFFFFFu
			, uint32_t reference = 0u );
		/**
		*\~french
		*\return
		*	Le VkPipelineDepthStencilStateCreateInfo engendré.
		*\~english
		*\return 
		*	The resulting VkPipelineDepthStencilStateCreateInfo.
		*/
		inline VkStencilOpState const & getState()const
		{
			return m_state;
		}

	private:
		VkStencilOpState m_state;
	};
}
