/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <common/Format.hpp>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if __linux__
typedef struct _XDisplay Display;
typedef struct xcb_connection_t xcb_connection_t;
typedef struct xcb_screen_t xcb_screen_t;
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_visualid_t;
typedef unsigned long XID;
typedef XID Window;
typedef XID Colormap;
typedef unsigned long VisualID;
typedef struct _XDisplay Display;
typedef void * EGLDisplay;
typedef void * EGLContext;
typedef void * EGLSurface;
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vulkan_xcb.h>
#elif _WIN32
#include <vulkan/vulkan_win32.h>
#endif

namespace ashes::gl
{
	PFN_vkVoidFunction getFunction( char const * const name );

	template< typename FuncT >
	inline bool getFunction( char const * const name, FuncT & function )
	{
		function = FuncT( getFunction( name ) );
		return function != nullptr;
	}

	template< typename FuncT >
	inline bool getFunction( std::string const & name, FuncT & function )
	{
		function = FuncT( getFunction( name.c_str() ) );
		return function != nullptr;
	}

	inline void * getBufferOffset( intptr_t value )
	{
		return reinterpret_cast< void * >( reinterpret_cast< uint8_t * >( 0u ) + value );
	}

	struct ContextState;
	class ContextImpl;
	class ExtensionsHandler;
	class RenderWindow;

	using ContextImplPtr = std::unique_ptr< ContextImpl >;
	using ContextStateArray = std::vector< ContextState >;
}
