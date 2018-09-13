/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Semaphore.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	Semaphore::Semaphore( Device const & device )
		: renderer::Semaphore{ device }
	{
	}

	Semaphore::~Semaphore()
	{
	}
}
