/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/D3D11CommandPool.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags flags )
		: renderer::CommandPool{ device, queueFamilyIndex, flags }
		, m_device{ device }
	{
	}

	CommandPool::~CommandPool()
	{
	}

	renderer::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
