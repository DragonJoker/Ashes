/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Fence.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	Fence::Fence( Device const & device
		, renderer::FenceCreateFlags flags )
		: renderer::Fence{ device, flags }
	{
	}

	Fence::~Fence()
	{
	}

	renderer::WaitResult Fence::wait( uint64_t timeout )const
	{
		return renderer::WaitResult::eSuccess;
	}

	void Fence::reset()const
	{
	}
}
