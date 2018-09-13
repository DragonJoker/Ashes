/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/D3D11Connection.hpp"

#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Renderer.hpp"

#include <Core/PlatformWindowHandle.hpp>

namespace d3d11_renderer
{
	Connection::Connection( Renderer const & renderer
		, uint32_t deviceIndex
		, renderer::WindowHandle && handle )
		: renderer::Connection{ renderer, deviceIndex, std::move( handle ) }
		, m_renderer{ renderer }
		, m_gpu{ static_cast< PhysicalDevice const & >( renderer.getPhysicalDevice( deviceIndex ) ) }
	{
	}

	Connection::~Connection()
	{
	}
}
