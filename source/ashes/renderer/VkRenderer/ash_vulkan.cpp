#define ASHES_VK_PROTOTYPES
#include <ashes/ashes.h>

#include <common/DynamicLibrary.hpp>

#include <memory>
#include <vector>

namespace ashes::vk
{
	namespace
	{
		VkBool32 checkSupport( AshPluginDescription const & description )
		{
			VkBool32 result{ VK_FALSE };

			if ( description.functions.CreateInstance )
			{
				VkInstanceCreateInfo instanceInfo
				{
					VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
					nullptr,
					0u,
					nullptr,
					0u,
					nullptr,
					0u,
					nullptr,
				};
				VkInstance instance{ VK_NULL_HANDLE };
				auto res = description.functions.CreateInstance( &instanceInfo, nullptr, &instance );

				if ( instance )
				{
					if ( description.functions.EnumeratePhysicalDevices )
					{
						uint32_t gpuCount{ 0u };
						description.functions.EnumeratePhysicalDevices( instance
							, &gpuCount
							, nullptr );
						result = gpuCount ? VK_TRUE : VK_FALSE;
					}

					if ( description.functions.DestroyInstance )
					{
						description.functions.DestroyInstance( instance, nullptr );
					}
				}
			}

			return result;
		}
	}

	struct VkLibrary
	{
		AshPluginDescription description
		{
			"vk",
			"Vulkan renderer for Ashes",
		};
		std::unique_ptr< ashes::DynamicLibrary > library;

		VkResult init()
		{
			VkResult result = description.getInstanceProcAddr
				? VK_SUCCESS
				: VK_ERROR_INITIALIZATION_FAILED;

			if ( result != VK_SUCCESS )
			{
#if defined( _WIN32 )
				static char const * const libraryName = "vulkan-1.dll";
#elif defined( __linux__ )
				static char const * const libraryName = "libvulkan.so.1";
#else
#	error Unsupported platform
#endif

				auto vklibrary = std::make_unique< ashes::DynamicLibrary >( libraryName );
				vklibrary->getFunction( "vkGetInstanceProcAddr", description.getInstanceProcAddr );
				description.features =
				{
					true, // hasTexBufferRange
					true, // hasImageTexture
					true, // hasBaseInstance
					true, // hasClearTexImage
					true, // hasComputeShaders
					true, // hasStorageBuffers
					true, // supportsPersistentMapping
				};
#define VK_LIB_GLOBAL_FUNCTION( x )\
				vklibrary->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_INSTANCE_FUNCTION( x )\
				vklibrary->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_DEVICE_FUNCTION( x )\
				vklibrary->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_GLOBAL_FUNCTION_EXT( n, x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( n, x )
#define VK_LIB_DEVICE_FUNCTION_EXT( n, x )
#	include <ashes/ashes_functions_list.hpp>
				result = VK_SUCCESS;

				description.support.priority = 10u;
				description.support.supported = checkSupport( description );
				library = std::move( vklibrary );
			}

			return result;
		}
	};

	VkLibrary & getLibrary()
	{
		thread_local VkLibrary library;
		return library;
	}
}

#if defined( VK_USE_PLATFORM_WIN32_KHR )
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
		auto result = ashes::vk::getLibrary().init();

		if ( result == VK_SUCCESS )
		{
			*pDescription = ashes::vk::getLibrary().description;
		}

		return result;
	}
}
