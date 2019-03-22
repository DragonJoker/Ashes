/**
*\file
*	VkCreateInstance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/Core/Instance.hpp>

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
	VkRenderer_API ashes::Result createInstance( ashes::InstanceCreateInfo createInfo
		, ashes::Instance ** instance );
	/**
	*\~french
	*\brief
	*	Récupère la version de l'instance.
	*\~english
	*\brief
	*	Retrieves the instance version.
	*/
	VkRenderer_API ashes::Result enumerateVersion( uint32_t * version );
	/**
	*\~french
	*\brief
	*	Enumère les couches de l'instance.
	*\~english
	*\brief
	*	Enumerates the instance layers.
	*/
	VkRenderer_API ashes::Result enumerateLayerProperties( uint32_t * count
		, ashes::LayerProperties * props );
	/**
	*\~french
	*\brief
	*	Enumère les extensions de la couche d'instance donnée.
	*\~english
	*\brief
	*	Enumerates the extension for the given instance layer.
	*/
	VkRenderer_API ashes::Result enumerateExtensionProperties( char const * const layerName
		, uint32_t * count
		, ashes::ExtensionProperties * props );
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
