/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestConnection.hpp"

#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace test_renderer
{
	Connection::Connection( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Connection{ instance, gpu, std::move( handle ) }
	{
	}

	bool Connection::getSurfaceSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}
}
