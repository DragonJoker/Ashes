/**
*\file
*	VkCreateRenderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Renderer/Renderer.hpp>

#ifdef _WIN32
#	ifdef VkRenderer_EXPORTS
#		define VkRenderer_API __declspec( dllexport )
#	else
#		define VkRenderer_API __declspec( dllimport )
#	endif
#else
#	define GlRenderer_API
#endif

namespace vk_renderer
{
	/**
	*\brief
	*	Crée un renderer Vulkan.
	*\param[in] connection
	*	La connection avec la fenêtre.
	*/
	VkRenderer_API renderer::RendererPtr createRenderer();
}
