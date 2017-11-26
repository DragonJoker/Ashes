/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/TessellationState.hpp>

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Wrapper de vk::TessellationState.
	*/
	class TessellationState
	{
	public:
		TessellationState( TessellationStateFlags flags
			, uint32_t patchControlPoints );
		/**
		*\return
		*	Le vk::TessellationState.
		*/
		inline vk::TessellationState const & getState()const
		{
			return m_state;
		}

	private:
		vk::TessellationState m_state;
	};
}
