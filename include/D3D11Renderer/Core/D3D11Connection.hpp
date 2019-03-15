/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/Connection.hpp>

namespace d3d11_renderer
{
	class Connection
		: public ashes::Connection
	{
	public:
		Connection( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );
		~Connection();

		bool getSurfaceSupport( uint32_t queueFamilyIndex )const override;
	};
}
