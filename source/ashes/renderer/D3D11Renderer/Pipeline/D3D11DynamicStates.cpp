#include "Pipeline/D3D11DynamicStates.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	namespace
	{
		VkDynamicStateArray getStates( VkPipelineDynamicStateCreateInfo const * createInfo )
		{
			return createInfo
				? makeVector( createInfo->pDynamicStates, createInfo->dynamicStateCount )
				: VkDynamicStateArray{};
		}

		VkPipelineDynamicStateCreateInfo getState( VkPipelineDynamicStateCreateInfo const * createInfo )
		{
			return ( createInfo
				? *createInfo
				: VkPipelineDynamicStateCreateInfo{} );
		}
	}

	DynamicStates::DynamicStates( VkPipelineDynamicStateCreateInfo const * createInfo )
		: m_dynamicStates{ getStates( createInfo ) }
		, m_dynamicState{ getState( createInfo ) }
	{
		m_dynamicState.pDynamicStates = m_dynamicStates.data();

		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_VIEWPORT ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_VIEWPORT, true );
			m_viewport = VkViewport{};
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_SCISSOR ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_SCISSOR, true );
			m_scissor = VkRect2D{};
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_LINE_WIDTH, true );
			m_lineWidth = 1.0f;
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_DEPTH_BIAS, true );
			m_depthBias = DepthBias{};
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_BLEND_CONSTANTS ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_BLEND_CONSTANTS, true );
			m_blendConstants = std::array< float, 4u >{};
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BOUNDS ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_DEPTH_BOUNDS, true );
			m_depthBounds = DepthBounds{ 0.0f, 1.0f };
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK, true );
			m_stencilCompareMask[0] = uint32_t( ~0u );
			m_stencilCompareMask[1] = uint32_t( ~0u );
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK, true );
			m_stencilWriteMask[0] = uint32_t( ~0u );
			m_stencilWriteMask[1] = uint32_t( ~0u );
		}
		if ( hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_REFERENCE ) )
		{
			m_dirtyStates.emplace( VK_DYNAMIC_STATE_STENCIL_REFERENCE, true );
			m_stencilReference[0] = 0u;
			m_stencilReference[1] = 0u;
		}
	}

	void DynamicStates::setLineWidth( Optional< float > const & lineWidth )
	{
		if ( lineWidth.has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_LINE_WIDTH] = m_dirtyStates[VK_DYNAMIC_STATE_LINE_WIDTH]
				|| ( m_lineWidth.value() != lineWidth.value() );
			m_lineWidth = lineWidth;
		}
	}

	void DynamicStates::setDepthBias( Optional< DepthBias > const & depthBias )
	{
		if ( depthBias.has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_DEPTH_BIAS] = m_dirtyStates[VK_DYNAMIC_STATE_DEPTH_BIAS]
				|| ( m_depthBias.value() != depthBias.value() );
			m_depthBias = depthBias;
		}
	}

	void DynamicStates::setBlendConstants( Optional< std::array< float, 4u > > const & blendConstants )
	{
		if ( blendConstants.has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_BLEND_CONSTANTS ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_BLEND_CONSTANTS] = m_dirtyStates[VK_DYNAMIC_STATE_BLEND_CONSTANTS]
				|| ( m_blendConstants.value() != blendConstants.value() );
			m_blendConstants = blendConstants;
		}
	}

	void DynamicStates::setDepthBounds( Optional< DepthBounds > const & depthBounds )
	{
		if ( depthBounds.has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BOUNDS ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_DEPTH_BOUNDS] = m_dirtyStates[VK_DYNAMIC_STATE_DEPTH_BOUNDS]
				|| ( m_depthBounds.value() != depthBounds.value() );
			m_depthBounds = depthBounds;
		}
	}

	void DynamicStates::setStencilCompareMask( std::array< Optional< uint32_t >, 2u > const & stencilCompareMask )
	{
		if ( stencilCompareMask[0].has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK] = m_dirtyStates[VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK]
				|| ( m_stencilCompareMask[0].value() != stencilCompareMask[0].value() )
				|| ( m_stencilCompareMask[1].value() != stencilCompareMask[1].value() );
			m_stencilCompareMask = stencilCompareMask;
		}
	}

	void DynamicStates::setStencilWriteMask( std::array< Optional< uint32_t >, 2u > const & stencilWriteMask )
	{
		if ( stencilWriteMask[0].has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_STENCIL_WRITE_MASK] = m_dirtyStates[VK_DYNAMIC_STATE_STENCIL_WRITE_MASK]
				|| ( m_stencilWriteMask[0].value() != stencilWriteMask[0].value() )
				|| ( m_stencilWriteMask[1].value() != stencilWriteMask[1].value() );
			m_stencilWriteMask = stencilWriteMask;
		}
	}

	void DynamicStates::setStencilReference( std::array< Optional< uint32_t >, 2u > const & stencilReference )
	{
		if ( stencilReference[0].has_value()
			&& hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_REFERENCE ) )
		{
			m_dirtyStates[VK_DYNAMIC_STATE_STENCIL_REFERENCE] = m_dirtyStates[VK_DYNAMIC_STATE_STENCIL_REFERENCE]
				|| ( m_stencilReference[0].value() != stencilReference[0].value() )
				|| ( m_stencilReference[1].value() != stencilReference[1].value() );
			m_stencilReference = stencilReference;
		}
	}
}
