/**
*\file
*	TestCreateInstance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/Core/Instance.hpp>

#if defined( _WIN32 ) && !defined( TestRenderer_STATIC )
#	ifdef TestRenderer_EXPORTS
#		define TestRenderer_API __declspec( dllexport )
#	else
#		define TestRenderer_API __declspec( dllimport )
#	endif
#else
#	define TestRenderer_API
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
	TestRenderer_API ashes::Result createInstance( ashes::InstanceCreateInfo createInfo
		, ashes::Instance ** instance );
	/**
	*\~french
	*\brief
	*	Récupère la version de l'instance.
	*\~english
	*\brief
	*	Retrieves the instance version.
	*/
	TestRenderer_API ashes::Result enumerateVersion( uint32_t * version );
	/**
	*\~french
	*\brief
	*	Enumère les couches de l'instance.
	*\~english
	*\brief
	*	Enumerates the instance layers.
	*/
	TestRenderer_API ashes::Result enumerateLayerProperties( uint32_t * count
		, VkLayerProperties * props );
	/**
	*\~french
	*\brief
	*	Enumère les extensions de la couche d'instance donnée.
	*\~english
	*\brief
	*	Enumerates the extension for the given instance layer.
	*/
	TestRenderer_API ashes::Result enumerateExtensionProperties( char const * const layerName
		, uint32_t * cont
		, VkExtensionProperties * props );
	/**
	*\~french
	*\return
	*	Le nom court du renderer.
	*\~english
	*\return
	*	The renderer short name.
	*/
	TestRenderer_API char const * getShortName();
	/**
	*\~french
	*\return
	*	Le nom complet du renderer.
	*\~english
	*\return
	*	The renderer full name.
	*/
	TestRenderer_API char const * getFullName();
}
