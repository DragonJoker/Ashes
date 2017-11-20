/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_RendererPrerequisites_HPP___
#define ___Renderer_RendererPrerequisites_HPP___
#pragma once

#include "PixelFormat.hpp"

#include <time.h>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>

namespace renderer
{
	/**
	*@~french
	*@brief
	*	Convertit un utils::PixelFormat en VkFormat.
	*@param[in] format
	*	Le utils::PixelFormat.
	*@return
	*	Le format Vulkan.
	*@~english
	*@brief
	*	Converts a utils::PixelFormat to a VkFormat.
	*@param[in] format
	*	The utils::PixelFormat.
	*@return
	*	The Vulkan format.
	*/
	VkFormat convert( PixelFormat format );
}

#endif
