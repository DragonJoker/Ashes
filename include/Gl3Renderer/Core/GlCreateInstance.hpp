/**
*\file
*	GlCreateInstance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/Core/Instance.hpp>

#if defined( _WIN32 ) && !defined( Gl3Renderer_STATIC )
#	ifdef Gl3Renderer_EXPORTS
#		define Gl3Renderer_API __declspec( dllexport )
#	else
#		define Gl3Renderer_API __declspec( dllimport )
#	endif
#else
#	define Gl3Renderer_API
#endif

extern "C"
{
	/**
	*\~french
	*\brief
	*	Crée un renderer OpenGL.
	*\param[in] configuration
	*	La configuration de création.
	*\~english
	*\brief
	*	Creates an OpenGL renderer.
	*\param[in] configuration
	*	The creation options.
	*/
	Gl3Renderer_API ashes::Instance * createInstance( ashes::Instance::Configuration const & configuration );
	/**
	*\~french
	*\return
	*	Le nom court du renderer.
	*\~english
	*\return
	*	The renderer short name.
	*/
	Gl3Renderer_API char const * getShortName();
	/**
	*\~french
	*\return
	*	Le nom complet du renderer.
	*\~english
	*\return
	*	The renderer full name.
	*/
	Gl3Renderer_API char const * getFullName();
}
