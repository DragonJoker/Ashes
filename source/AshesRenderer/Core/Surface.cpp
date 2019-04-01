/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Core/Surface.hpp"

namespace ashes
{
	Surface::Surface( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, WindowHandle handle )
		: m_handle{ std::move( handle ) }
		, m_gpu{ gpu }
	{
	}
}
