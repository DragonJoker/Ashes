#include <ashes/ashes.h>

#include <ashes/common/DynamicLibrary.hpp>
#include <ashes/common/FileUtils.hpp>

#include <cassert>
#include <iostream>
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
#if defined( __APPLE__ )
#	if VK_KHR_portability_enumeration
				char const * extensions = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
				instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
				instanceInfo.enabledExtensionCount = 1u;
				instanceInfo.ppEnabledExtensionNames = &extensions;
#	endif
#endif
				VkInstance instance{ nullptr };
				auto res = description.functions.CreateInstance( &instanceInfo, nullptr, &instance );

				if ( res == VK_SUCCESS && instance )
				{
					if ( description.functions.EnumeratePhysicalDevices )
					{
						uint32_t gpuCount{ 0u };
						res = description.functions.EnumeratePhysicalDevices( instance
							, &gpuCount
							, nullptr );
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
						result = VkBool32( ( res == VK_SUCCESS && gpuCount )
							? VK_TRUE
							: VK_FALSE );
#pragma GCC diagnostic pop
					}

					if ( description.functions.DestroyInstance )
					{
						description.functions.DestroyInstance( instance, nullptr );
					}
				}
			}

			return result;
		}

		struct VkLibrary
		{
			AshPluginDescription description{ "vk"
				, "Vulkan renderer for Ashes"
				, {}
				, {}
				, {}
				, {}
			, ASHPLUGIN_DROPIN };
			std::unique_ptr< ashes::DynamicLibrary > library;

			VkResult init( AshPluginMode mode )
			{
				auto validMode = ( description.mode == ASHPLUGIN_UNDEFINED || description.mode == mode );
				assert( validMode
					&& "ashesTestRenderer: Plugin mode has already been selected." );
				VkResult result = description.getInstanceProcAddr
					? ( validMode ? VK_SUCCESS : VK_ERROR_INCOMPATIBLE_DRIVER )
					: VK_ERROR_INITIALIZATION_FAILED;

				if ( result == VK_ERROR_INITIALIZATION_FAILED )
				{
#if defined( _WIN32 )
					static std::string_view const libraryName = "vulkan-1.dll";
#elif defined( __linux__ )
					static std::string_view const libraryName = "libvulkan.so.1";
#elif defined( __APPLE__ )
					static std::string_view const libraryName = "libvulkan.1.dylib";
#else
#	error Unsupported platform
#endif

					std::unique_ptr< ashes::DynamicLibrary > vklibrary;
					ashes::lookForSharedLibrary( [&vklibrary]( std::string const & folder
						, std::string const & name )
						{
							if ( libraryName == name
								&& !vklibrary )
							{
								vklibrary = std::make_unique< ashes::DynamicLibrary >( folder / name );
								PFN_ashSelectPlugin selectPlugin;

								if ( vklibrary->getFunction( "ashSelectPlugin", selectPlugin ) )
								{
									vklibrary.reset();
								}
							}

							return vklibrary != nullptr;
						} );

					if ( vklibrary )
					{
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
							{}, // maxShaderLanguageVersion
						};
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
					vklibrary->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_INSTANCE_FUNCTION( v, x )\
					vklibrary->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_DEVICE_FUNCTION( v, x )\
					vklibrary->getFunction( "vk"#x, description.functions.x );
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )
#	include <ashes/ashes_functions_list.hpp>
						result = VK_SUCCESS;

						description.mode = mode;
						description.support.priority = 10u;
						description.support.supported = checkSupport( description );
						library = std::move( vklibrary );
					}
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
}

#if defined( _WIN32 )
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
	VkRenderer_API PFN_vkVoidFunction VKAPI_PTR vk_icdGetInstanceProcAddr( VkInstance instance
		, const char * name );
	VkRenderer_API PFN_vkVoidFunction VKAPI_PTR vk_icdGetPhysicalDeviceProcAddr( VkInstance instance
		, const char * name );
	VkRenderer_API VkResult VKAPI_PTR vk_icdNegotiateLoaderICDInterfaceVersion( uint32_t * pVersion );
	VkRenderer_API VkResult VKAPI_PTR ashGetPluginDescription( AshPluginDescription * pDescription );

#pragma region ICD mode

	VkRenderer_API PFN_vkVoidFunction VKAPI_PTR vk_icdGetInstanceProcAddr( [[maybe_unused]] VkInstance instance
		, [[maybe_unused]] const char * name )
	{
		std::cerr << "ashesVkRenderer: Unsupported ICD mode" << std::endl;
		assert( false && "ashesVkRenderer: Unsupported ICD mode" );
		return nullptr;
	}

	VkRenderer_API PFN_vkVoidFunction VKAPI_PTR vk_icdGetPhysicalDeviceProcAddr( [[maybe_unused]] VkInstance instance
		, [[maybe_unused]] const char * name )
	{
		std::cerr << "ashesVkRenderer: Unsupported ICD mode" << std::endl;
		assert( false && "ashesVkRenderer: Unsupported ICD mode" );
		return nullptr;
	}

	VkRenderer_API VkResult VKAPI_PTR vk_icdNegotiateLoaderICDInterfaceVersion( [[maybe_unused]] uint32_t * pVersion )
	{
		std::cerr << "ashesVkRenderer: Unsupported ICD mode" << std::endl;
		assert( false && "ashesVkRenderer: Unsupported ICD mode" );
		return VK_ERROR_INCOMPATIBLE_DRIVER;
	}

#pragma endregion
#pragma region Drop-in replacement mode

	VkRenderer_API VkResult VKAPI_PTR ashGetPluginDescription( AshPluginDescription * pDescription )
	{
		auto result = ashes::vk::getLibrary().init( ASHPLUGIN_DROPIN );

		if ( result == VK_SUCCESS )
		{
			*pDescription = ashes::vk::getLibrary().description;
		}

		return result;
	}
}
