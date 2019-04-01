/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_C_H___
#define ___Ashes_C_H___
#pragma once

#include "AshesC/ash_platform.h"

#if ASHES_ANDROID
#	define VK_USE_PLATFORM_ANDROID_KHR 1
#elif ASHES_IOS
#	define VK_USE_PLATFORM_IOS_MVK 1
#elif ASHES_MACOS
#	define VK_USE_PLATFORM_MACOS_MVK 1
#elif ASHES_MIR
#	define VK_USE_PLATFORM_MIR_KHR 1
#elif ASHES_VI
#	define VK_USE_PLATFORM_VI_NN 1
#elif ASHES_WAYLAND
#	define VK_USE_PLATFORM_WAYLAND_KHR 1
#elif ASHES_WIN32
#	define VK_USE_PLATFORM_WIN32_KHR 1
#elif ASHES_XCB
#	define VK_USE_PLATFORM_XCB_KHR 1
#elif ASHES_XLIB
#	define VK_USE_PLATFORM_XLIB_KHR 1
#elif ASHES_XLIB_XRANDR
#	define VK_USE_PLATFORM_XLIB_XRANDR_EXT 1
#endif

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#if ASHES_WIN32
#	ifdef AshesC_EXPORTS
#		define Ashes_API __declspec( dllexport )
#	else
#		define Ashes_API __declspec( dllimport )
#	endif
#else
#	define Ashes_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	Ashes_API PFN_vkVoidFunction vkGetInstanceProcAddr( VkInstance instance
		, const char * name );

	typedef struct AshPluginDescription
	{
		char name[16];
		char description[64];
		PFN_vkGetInstanceProcAddr getInstanceProcAddr;
	} AshPluginDescription;

	typedef void( VKAPI_PTR * PFN_ashGetPluginsDescriptions )( uint32_t *, AshPluginDescription * );
	typedef VkResult( VKAPI_PTR * PFN_ashGetPluginDescription )( AshPluginDescription * );

	Ashes_API void VKAPI_PTR ashGetPluginsDescriptions( uint32_t * count
		, AshPluginDescription * pDescriptions );
	Ashes_API VkResult VKAPI_PTR ashGetCurrentPluginDescription( AshPluginDescription * description );

#ifdef __cplusplus
}
#endif

#endif
