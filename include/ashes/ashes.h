/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_C_H___
#define ___Ashes_C_H___
#pragma once

#include "ashes/ash_platform.h"

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
		/**
		*\brief
		*	Whether or not the plugin supports texture buffer ranges.
		*/
		VkBool32 hasBufferRange;
		/**
		*\brief
		*	Whether or not the plugin supports image textures.
		*/
		VkBool32 hasImageTexture;
		/**
		*\brief
		*	Whether or not the plugin supports vertex base instance.
		*/
		VkBool32 hasBaseInstance;
		/**
		*\brief
		*	Whether or not the plugin supports clearing of single images.
		*/
		VkBool32 hasClearTexImage;
		/**
		*\brief
		*	Whether or not the plugin supports compute shaders.
		*/
		VkBool32 hasComputeShaders;
		/**
		*\brief
		*	Whether or not the plugin supports shader storage buffers.
		*/
		VkBool32 hasStorageBuffers;
		/**
		*\brief
		*	Whether or not the plugin supports persistent mapping.
		*/
		VkBool32 supportsPersistentMapping;
	} AshPluginFeatures;

	typedef struct AshPluginSupport
	{
		/**
		*\brief
		*	The plugin's priority (from 1 to 10).
		*/
		uint32_t priority;
		/**
		*\brief
		*	Whether or not the plugin is supported.
		*/
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
#include <ashes/common/VulkanFunctionsList.inl>
	} AshPluginStaticFunction;

	typedef struct AshPluginDescription
	{
		/**
		*\brief
		*	The plugin's short name.
		*/
		char name[16];
		/**
		*\brief
		*	The plugin's description.
		*/
		char description[64];
		/**
		*\brief
		*	The plugin's main entry point.
		*/
		PFN_vkGetInstanceProcAddr getInstanceProcAddr;
		/**
		*\brief
		*	The plugin's supported features.
		*/
		AshPluginFeatures features;
		/**
		*\brief
		*	The plugin's static functions (for static loader support).
		*/
		AshPluginStaticFunction functions;
		/**
		*\brief
		*	The plugin's support informations.
		*/
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
