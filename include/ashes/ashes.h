/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_C_H___
#define ___Ashes_C_H___
#pragma once

#include "ashes/ash_platform.h"

#ifndef ASHES_VK_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
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
	typedef struct AshPluginFeatures
	{
		VkBool32 hasTexBufferRange;
		VkBool32 hasImageTexture;
		VkBool32 hasBaseInstance;
		VkBool32 hasClearTexImage;
		VkBool32 hasComputeShaders;
		VkBool32 hasStorageBuffers;
		VkBool32 supportsPersistentMapping;
	} AshPluginFeatures;

	typedef struct AshPluginSupport
	{
		uint32_t priority;
		VkBool32 supported;
	} AshPluginSupport;

	typedef struct AshPluginStaticFunction
	{
#define VK_LIB_GLOBAL_FUNCTION( x )\
		PFN_vk##x x;
#define VK_LIB_INSTANCE_FUNCTION( x )\
		PFN_vk##x x;
#define VK_LIB_DEVICE_FUNCTION( x )\
		PFN_vk##x x;
#define VK_LIB_GLOBAL_FUNCTION_EXT( x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( x )
#define VK_LIB_DEVICE_FUNCTION_EXT( x )
#include <AshesCommon/VulkanFunctionsList.inl>
	} AshPluginStaticFunction;

	typedef struct AshPluginDescription
	{
		char name[16];
		char description[64];
		PFN_vkGetInstanceProcAddr getInstanceProcAddr;
		AshPluginFeatures features;
		AshPluginStaticFunction functions;
		AshPluginSupport support;
	} AshPluginDescription;

	typedef void( VKAPI_PTR * PFN_ashEnumeratePluginsDescriptions )( uint32_t *, AshPluginDescription * );
	typedef VkResult( VKAPI_PTR * PFN_ashGetPluginDescription )( AshPluginDescription * );
	typedef VkResult( VKAPI_PTR * PFN_ashSelectPlugin )( AshPluginDescription );

	Ashes_API void VKAPI_PTR ashEnumeratePluginsDescriptions( uint32_t * count
		, AshPluginDescription * pDescriptions );
	Ashes_API VkResult VKAPI_PTR ashSelectPlugin( AshPluginDescription description );
	Ashes_API VkResult VKAPI_PTR ashGetCurrentPluginDescription( AshPluginDescription * description );

#ifdef __cplusplus
}
#endif

#endif
