/**
*\file
*	TestCreateRenderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Core/Renderer.hpp>

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
	*\param[in] configuration
	*	La configuration de création.
	*\~english
	*\brief
	*	Creates a D3D11 renderer.
	*\param[in] configuration
	*	The creation options.
	*/
	D3D11Renderer_API ashes::Renderer * createRenderer( ashes::Renderer::Configuration const & configuration );
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
