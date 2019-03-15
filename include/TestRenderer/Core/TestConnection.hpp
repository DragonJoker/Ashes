/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Core/Connection.hpp>

namespace test_renderer
{
	class Connection
		: public ashes::Connection
	{
	public:
		Connection( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );

		bool getSurfaceSupport( uint32_t queueFamilyIndex )const override;
	};
}
