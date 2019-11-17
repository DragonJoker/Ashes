/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DynamicStates_HPP___
#define ___D3D11Renderer_DynamicStates_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class DynamicStates
	{
	public:
		DynamicStates( VkDevice device
			, VkPipelineDynamicStateCreateInfo const * createInfo );
		~DynamicStates();

		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			return m_dynamicStates.end() != std::find( m_dynamicStates.begin()
				, m_dynamicStates.end()
				, state );
		}

	private:
		VkDynamicStateArray m_dynamicStates;
		VkPipelineDynamicStateCreateInfo m_dynamicState;
	};
}

#endif
