/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestConnection.hpp"

#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestRenderer.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace test_renderer
{
	Connection::Connection( Renderer const & renderer
		, uint32_t deviceIndex
		, ashes::WindowHandle && handle )
		: ashes::Connection{ renderer, deviceIndex, std::move( handle ) }
		, m_renderer{ renderer }
		, m_gpu{ static_cast< PhysicalDevice const & >( renderer.getPhysicalDevice( deviceIndex ) ) }
	{
	}

	Connection::~Connection()
	{
	}
}
