/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

#include <Ashes/Core/Exception.hpp>

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
			throw ashes::Exception{ ashes::Result( errCode ), text };
		}
	}
}
