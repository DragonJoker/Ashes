/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Semaphore
	{
	public:
		explicit Semaphore( VkDevice device );
		~Semaphore();
	};
}
