/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <string>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Vérifie que le code donné ne représente pas une erreur.
	*\remarks
	*	Lance une ashes::Exception si un problème est détecté.
	*\param[in] result
	*	Le code résultat Vulkan.
	*\~english
	*\brief
	*	Checks if the given code is an error code.
	*\remarks
	*	Throws a ashes::Exception if a problem is detected.
	*\param[in] result
	*	The Vulkan result code.
	*/
	void checkError( VkResult result
		, char const * const text );
}
