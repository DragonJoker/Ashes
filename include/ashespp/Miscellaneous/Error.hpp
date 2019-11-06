/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Checks if the given code is an error code.
	*\remarks
	*	Throws an ashes::Exception if a problem is detected.
	*\param[in] result
	*	The Vulkan result code.
	*/
	void checkError( VkResult result
		, char const * const text );
}
