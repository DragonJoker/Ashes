/**
*\file
*	TestCreateRenderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Core/Renderer.hpp>

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
	TestRenderer_API renderer::Renderer * createRenderer( renderer::Renderer::Configuration const & configuration );
}
