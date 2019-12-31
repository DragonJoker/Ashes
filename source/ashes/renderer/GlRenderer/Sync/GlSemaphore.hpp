/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Semaphore
		: public AutoIdIcdObject< Semaphore >
	{
	public:
		explicit Semaphore( VkDevice device );
	};
}
