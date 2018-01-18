/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_PixelFormat_HPP___
#define ___Renderer_PixelFormat_HPP___
#pragma once

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Dit si le format Vulkan donné est un format utilisable pour les tampons de profondeur et stencil.
	*\param[in] format
	*	Le format Vulkan à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given Vulkan format is usable in depth and stencil buffers.
	*\param[in] format
	*	The Vulkan format.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	bool isDepthStencilFormat( renderer::PixelFormat format );
	/**
	*\~french
	*\brief
	*	Dit si le format Vulkan donné est un format utilisable pour les tampons de stencil.
	*\param[in] format
	*	Le format Vulkan à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given Vulkan format is usable in stencil buffers.
	*\param[in] format
	*	The Vulkan format.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	bool isStencilFormat( renderer::PixelFormat format );
	/**
	*\~french
	*\brief
	*	Dit si le format Vulkan donné est un format utilisable pour les tampons de profondeur.
	*\param[in] format
	*	Le format Vulkan à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given Vulkan format is usable in depth buffers.
	*\param[in] format
	*	The Vulkan format.
	*\return
	*	\p true if it is usable in depth buffers.
	*/
	bool isDepthFormat( renderer::PixelFormat format );
}

#endif
