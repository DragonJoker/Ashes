/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/AshesPPPrerequisites.hpp"

#include <Ashes/Core/Exception.hpp>

#include <map>
#include <atomic>
#include <thread>

namespace ashespp
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
