/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_DynamicStates_HPP___
#define ___XBOXRenderer_DynamicStates_HPP___
#pragma once

#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class DynamicStates
	{
	public:
		explicit DynamicStates( VkPipelineDynamicStateCreateInfo const * createInfo );

		void setLineWidth( Optional< float > const & lineWidth );
		void setDepthBias( Optional< DepthBias > const & depthBias );
		void setBlendConstants( Optional< std::array< float, 4u > > const & blendConstants );
		void setDepthBounds( Optional< DepthBounds > const & depthBounds );
		void setStencilCompareMask( std::array< Optional< uint32_t >, 2u > const & stencilCompareMask );
		void setStencilWriteMask( std::array< Optional< uint32_t >, 2u > const & stencilWriteMask );
		void setStencilReference( std::array< Optional< uint32_t >, 2u > const & stencilReference );

		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			return m_dynamicStates.end() != std::find( m_dynamicStates.begin()
				, m_dynamicStates.end()
				, state );
		}

		inline bool isDirty( VkDynamicState state )const
		{
			auto it = m_dirtyStates.find( state );
			assert( it != m_dirtyStates.end() );
			return it->second;
		}

		inline DepthBias const & getDepthBias()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) );
			return m_depthBias.value();
		}

		inline uint32_t getFrontStencilCompareMask()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK ) );
			return m_stencilCompareMask[1].value();
		}

		inline uint32_t getBackStencilCompareMask()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK ) );
			return m_stencilCompareMask[0].value();
		}

		inline uint32_t getFrontStencilWriteMask()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK ) );
			return m_stencilWriteMask[1].value();
		}

		inline uint32_t getBackStencilWriteMask()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK ) );
			return m_stencilWriteMask[0].value();
		}

		inline uint32_t getFrontStencilReference()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_REFERENCE ) );
			return m_stencilReference[1].value();
		}

		inline uint32_t getBackStencilReference()
		{
			assert( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_REFERENCE ) );
			return m_stencilReference[0].value();
		}

	private:
		VkDynamicStateArray m_dynamicStates;
		VkPipelineDynamicStateCreateInfo m_dynamicState;
		std::map< VkDynamicState, bool > m_dirtyStates;
		Optional< VkViewport > m_viewport;
		Optional< VkRect2D > m_scissor;
		Optional< float > m_lineWidth;
		Optional< DepthBias > m_depthBias;
		Optional< std::array< float, 4u > > m_blendConstants;
		Optional< DepthBounds > m_depthBounds;
		std::array< Optional< uint32_t >, 2u > m_stencilCompareMask;
		std::array< Optional< uint32_t >, 2u > m_stencilWriteMask;
		std::array< Optional< uint32_t >, 2u > m_stencilReference;
	};
}

#endif
