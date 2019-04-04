#include <ashes.h>

#include <AshesCommon/DynamicLibrary.hpp>

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
		"Vulkan renderer for Ashes.",
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
				true,
				true,
				true,
				true,
				true,
				true,
				true,
			};
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
