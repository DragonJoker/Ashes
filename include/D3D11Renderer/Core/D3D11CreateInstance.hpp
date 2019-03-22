/**
*\file
*	TestCreateInstance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/Core/Instance.hpp>

#if defined( _WIN32 ) && !defined( D3D11Renderer_STATIC )
#	ifdef D3D11Renderer_EXPORTS
#		define D3D11Renderer_API __declspec( dllexport )
#	else
#		define D3D11Renderer_API __declspec( dllimport )
#	endif
#else
#	define D3D11Renderer_API
#endif

extern "C"
{
	/**
	*\~french
	*\brief
	*	Crée un renderer D3D11.
	*\param[in] createInfo
	*	La configuration de création.
	*\~english
	*\brief
	*	Creates a D3D11 renderer.
	*\param[in] createInfo
	*	The creation options.
	*/
	D3D11Renderer_API ashes::Result createInstance( ashes::InstanceCreateInfo createInfo
		, ashes::Instance ** instance );
	/**
	*\~french
	*\brief
	*	Récupère la version de l'instance.
	*\~english
	*\brief
	*	Retrieves the instance version.
	*/
	D3D11Renderer_API ashes::Result enumerateVersion( uint32_t * version );
	/**
	*\~french
	*\brief
	*	Enumère les couches de l'instance.
	*\~english
	*\brief
	*	Enumerates the instance layers.
	*/
	D3D11Renderer_API ashes::Result enumerateLayerProperties( uint32_t * count
		, ashes::LayerProperties * props );
	/**
	*\~french
	*\brief
	*	Enumère les extensions de la couche d'instance donnée.
	*\~english
	*\brief
	*	Enumerates the extension for the given instance layer.
	*/
	D3D11Renderer_API ashes::Result enumerateExtensionProperties( char const * const layerName
		, uint32_t * cont
		, ashes::ExtensionProperties * props );
	/**
	*\~french
	*\return
	*	Le nom court du renderer.
	*\~english
	*\return
	*	The renderer short name.
	*/
	D3D11Renderer_API char const * getShortName();
	/**
	*\~french
	*\return
	*	Le nom complet du renderer.
	*\~english
	*\return
	*	The renderer full name.
	*/
	D3D11Renderer_API char const * getFullName();
}
