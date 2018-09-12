/**
*\file
*	VkCreateRenderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Core/Renderer.hpp>

#if defined( _WIN32 ) && !defined( VkRenderer_STATIC )
#	ifdef VkRenderer_EXPORTS
#		define VkRenderer_API __declspec( dllexport )
#	else
#		define VkRenderer_API __declspec( dllimport )
#	endif
#else
#	define VkRenderer_API
#endif

extern "C"
{
	/**
	*\~french
	*\brief
	*	Crée un renderer Vulkan.
	*\param[in] configuration
	*	La configuration de création.
	*\~english
	*\brief
	*	Creates an Vulkan renderer.
	*\param[in] configuration
	*	The creation options.
	*/
	VkRenderer_API renderer::Renderer * createRenderer( renderer::Renderer::Configuration const & configuration );
	/**
	*\~french
	*\return
	*	Le nom court du renderer.
	*\~english
	*\return
	*	The renderer short name.
	*/
	VkRenderer_API char const * getShortName();
	/**
	*\~french
	*\return
	*	Le nom complet du renderer.
	*\~english
	*\return
	*	The renderer full name.
	*/
	VkRenderer_API char const * getFullName();
}
