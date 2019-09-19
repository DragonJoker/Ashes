#define ASHES_VK_PROTOTYPES
#include <ashes/ashes.h>

#include <common/DynamicLibrary.hpp>

#include <memory>

#if defined( _WIN32 )
static char const * const libraryName = "vulkan-1.dll";
#elif defined( __linux__ )
static char const * const libraryName = "libvulkan.so.1";
#else
#	error Unsupported platform
#endif

struct VkLibrary
{
	std::unique_ptr< ashes::DynamicLibrary > library;
	AshPluginDescription description
	{
		"vk",
		"Vulkan renderer for Ashes",
	};

	VkResult init()
	{
		VkResult result = VK_SUCCESS;

		if ( !description.getInstanceProcAddr )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
			library = std::make_unique< ashes::DynamicLibrary >( libraryName );
			library->getFunction( "vkGetInstanceProcAddr", description.getInstanceProcAddr );
			result = description.getInstanceProcAddr
				? VK_SUCCESS
				: VK_ERROR_INITIALIZATION_FAILED;
			description.features =
			{
				true, // hasBufferRange
				true, // hasImageTexture
				true, // hasBaseInstance
				true, // hasClearTexImage
				true, // hasComputeShaders
				true, // hasStorageBuffers
				true, // supportsPersistentMapping
			};
#define VK_LIB_GLOBAL_FUNCTION( x )\
			library->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_INSTANCE_FUNCTION( x )\
			library->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_DEVICE_FUNCTION( x )\
			library->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_GLOBAL_FUNCTION_EXT( x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( x )
#define VK_LIB_DEVICE_FUNCTION_EXT( x )
#include <common/VulkanFunctionsList.inl>

			description.support.priority = 10u;
			description.support.supported = VK_TRUE;
		}

		return result;
	}
};

thread_local VkLibrary g_library;

#if ASHES_WIN32
#	ifdef VkRenderer_EXPORTS
#		define VkRenderer_API __declspec( dllexport )
#	else
#		define VkRenderer_API __declspec( dllimport )
#	endif
#else
#	define VkRenderer_API
#endif

extern "C"
{
	VkRenderer_API VkResult VKAPI_PTR ashGetPluginDescription( AshPluginDescription * pDescription )
	{
		auto result = g_library.init();

		if ( result == VK_SUCCESS )
		{
			*pDescription = g_library.description;
		}

		return result;
	}
}
