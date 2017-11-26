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
		StencilOpState( StencilOp failOp
			, StencilOp passOp
			, StencilOp depthFailOp
			, CompareOp compareOp
			, uint32_t compareMask
			, uint32_t writeMask
			, uint32_t reference );
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
