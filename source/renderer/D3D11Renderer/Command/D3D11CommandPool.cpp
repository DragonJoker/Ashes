/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/D3D11CommandPool.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags flags )
		: ashes::CommandPool{ device, queueFamilyIndex, flags }
		, m_device{ device }
	{
	}

	CommandPool::~CommandPool()
	{
	}

	ashes::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
