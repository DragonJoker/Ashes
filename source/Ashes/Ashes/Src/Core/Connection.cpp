/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/Connection.hpp"

#include "Core/Renderer.hpp"

namespace ashes
{
	Connection::Connection( Renderer const & renderer
		, uint32_t deviceIndex
		, WindowHandle && handle )
		: m_handle{ std::move( handle ) }
		, m_gpu{ renderer.getPhysicalDevice( deviceIndex ) }
		, m_surfaceCapabilities{}
	{
	}
}
