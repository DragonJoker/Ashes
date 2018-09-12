/**
*\file
*	GlCreateRenderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Core/Renderer.hpp>

#if defined( _WIN32 ) && !defined( GlRenderer_STATIC )
#	ifdef GlRenderer_EXPORTS
#		define GlRenderer_API __declspec( dllexport )
#	else
#		define GlRenderer_API __declspec( dllimport )
#	endif
#else
#	define GlRenderer_API
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
	GlRenderer_API renderer::Renderer * createRenderer( renderer::Renderer::Configuration const & configuration );
	/**
	*\~french
	*\return
	*	Le nom court du renderer.
	*\~english
	*\return
	*	The renderer short name.
	*/
	GlRenderer_API char const * getShortName();
	/**
	*\~french
	*\return
	*	Le nom complet du renderer.
	*\~english
	*\return
	*	The renderer full name.
	*/
	GlRenderer_API char const * getFullName();
}
