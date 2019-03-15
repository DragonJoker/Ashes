/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/Connection.hpp"

namespace ashes
{
	Connection::Connection( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, WindowHandle handle )
		: m_handle{ std::move( handle ) }
		, m_gpu{ gpu }
		, m_surfaceCapabilities{}
	{
	}
}
