#include "Pipeline/D3D11DynamicStates.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
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

	DynamicStates::DynamicStates( VkDevice device
		, VkPipelineDynamicStateCreateInfo const * createInfo )
		: m_dynamicStates{ getStates( createInfo ) }
		, m_dynamicState{ getState( createInfo ) }
	{
		m_dynamicState.pDynamicStates = m_dynamicStates.data();
	}

	DynamicStates::~DynamicStates()
	{
	}
}
