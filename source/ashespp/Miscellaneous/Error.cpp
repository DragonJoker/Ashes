/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/AshesPPPrerequisites.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	void checkError( VkResult errCode
		, char const * const text )
	{
		if ( bool result = ( errCode == VK_SUCCESS );
			!result )
		{
			throw Exception{ errCode, text };
		}
	}
}
