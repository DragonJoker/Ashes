/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

#include <Core/Exception.hpp>

#include <map>
#include <atomic>
#include <thread>

namespace vk_renderer
{
	void checkError( VkResult errCode
		, char const * const text )
	{
		bool result = errCode == VK_SUCCESS;

		if ( !result )
		{
			throw renderer::Exception{ renderer::Result( errCode ), text };
		}
	}
}
