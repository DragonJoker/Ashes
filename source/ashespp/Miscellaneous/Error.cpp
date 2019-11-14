/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/AshesPPPrerequisites.hpp"

#include <common/Exception.hpp>

#include <map>
#include <atomic>
#include <thread>

namespace ashes
{
	void checkError( VkResult errCode
		, char const * const text )
	{
		bool result = errCode == VK_SUCCESS;

		if ( !result )
		{
			throw Exception{ errCode, text };
		}
	}
}
