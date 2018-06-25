/*
This file belongs to RendererLib.
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
	*	Lance une renderer::Exception si un problème est détecté.
	*\param[in] result
	*	Le code résultat Vulkan.
	*\~english
	*\brief
	*	Checks if the given code is an error code.
	*\remarks
	*	Throws a renderer::Exception if a problem is detected.
	*\param[in] result
	*	The Vulkan result code.
	*/
	void checkError( VkResult result
		, char const * const text );
}
