/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/TestConnection.hpp"

#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestRenderer.hpp"

#include <Core/PlatformWindowHandle.hpp>

namespace test_renderer
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
