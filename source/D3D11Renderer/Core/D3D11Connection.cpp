/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Connection.hpp"

#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Instance.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace d3d11_renderer
{
	Connection::Connection( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Connection{ instance, gpu, std::move( handle ) }
	{
	}

	Connection::~Connection()
	{
	}

	bool Connection::getSurfaceSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}
}
