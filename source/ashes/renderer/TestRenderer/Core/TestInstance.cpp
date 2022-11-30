#include "Core/TestInstance.hpp"

#include "Core/TestSurface.hpp"
#include "Core/TestDebugReportCallback.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestSwapChain.hpp"

#include <ashes/common/Exception.hpp>
#include <ashes/ashes.hpp>

#include "ashestest_api.hpp"

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <cmath>
#include <iomanip>
#include <iostream>
#include <locale>
#pragma warning( pop )

namespace ashes::test
{
	namespace
	{
		VkApplicationInfo doGetDefaultApplicationInfo()
		{
			return
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,
				nullptr,
				nullptr,
				ashes::makeVersion( 1, 0, 0 ),
				nullptr,
				ashes::makeVersion( 1, 0, 0 ),
				ashes::makeVersion( 1, 0, 0 ),
			};
		}

		void doCheckEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
		{
			auto & available = getSupportedInstanceExtensions();

			for ( auto & extension : extensions )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&extension]( VkExtensionProperties const & lookup )
					{
						return lookup.extensionName == std::string{ extension };
					} ) )
				{
					throw ExtensionNotPresentException{ extension };
				}
			}
		}

		bool doHasEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
		{
			try
			{
				doCheckEnabledExtensions( extensions );
				return true;
			}
			catch ( ExtensionNotPresentException & )
			{
				return false;
			}
		}
	}

	VkPhysicalDeviceMemoryProperties const Instance::m_memoryProperties = []()
	{
		VkPhysicalDeviceMemoryProperties result{};
		// Emulate one device local heap
		result.memoryHeaps[result.memoryHeapCount++] = { ~( 0ull ), VK_MEMORY_HEAP_DEVICE_LOCAL_BIT };
		// and one host visible heap
		result.memoryHeaps[result.memoryHeapCount++] = { ~( 0ull ), 0u };

		// Emulate all combinations of device local memory types
		// and all combinations of host visible memory types
		result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, 0u };
		result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, 1u };

		return result;
	}();

	Instance::Instance( VkInstanceCreateInfo createInfo )
		: m_applicationInfo{ createInfo.pApplicationInfo ? *createInfo.pApplicationInfo : doGetDefaultApplicationInfo() }
		, m_flags{ createInfo.flags }
		, m_enabledLayerNames{ ashes::convert( CharPtrArray{ createInfo.ppEnabledLayerNames, createInfo.ppEnabledLayerNames + createInfo.enabledLayerCount } ) }
		, m_enabledExtensions{ ashes::convert( CharPtrArray{ createInfo.ppEnabledExtensionNames, createInfo.ppEnabledExtensionNames + createInfo.enabledExtensionCount } ) }
	{
		doInitialisePhysicalDevices();

		m_features.hasTexBufferRange = true;
		m_features.hasImageTexture = true;
		m_features.hasBaseInstance = true;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = true;
		m_features.hasStorageBuffers = true;
		m_features.supportsPersistentMapping = true;
	}

	Instance::~Instance()
	{
		for ( auto & physicalDevice : m_physicalDevices )
		{
			deallocate( physicalDevice, nullptr );
		}
	}

	uint32_t Instance::getApiVersion()const
	{
		return m_applicationInfo.apiVersion;
	}

	bool Instance::hasExtension( std::string_view extension )const
	{
		char const * const version = extension.data();
		return doHasEnabledExtensions( ashes::makeArrayView( &version, 1u ) );
	}

	VkPhysicalDeviceArray Instance::enumeratePhysicalDevices()const
	{
		return m_physicalDevices;
	}

	std::array< float, 16 > Instance::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[8] = ( right + left ) / ( right - left );
		result[9] = ( top + bottom ) / ( top - bottom );
		result[11] = float( -1 );
		result[10] = zFar / ( zNear - zFar );
		result[14] = -( zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		float const tanHalfFovy = float( tan( radiansFovY / float( 2 ) ) );

		std::array< float, 16 > result{ 0.0f };
		result[0] = 1.0f / ( aspect * tanHalfFovy );
		result[5] = 1.0f / tanHalfFovy;
		result[10] = zFar / ( zNear - zFar );
		result[11] = -1.0f;
		result[14] = zNear * zFar / ( zNear - zFar );

		return result;
	}

	std::array< float, 16 > Instance::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 2 ) / ( right - left );
		result[5] = float( 2 ) / ( top - bottom );
		result[12] = -( right + left ) / ( right - left );
		result[13] = -( top + bottom ) / ( top - bottom );
		result[10] = -float( 1 ) / ( zFar - zNear );
		result[14] = -zNear / ( zFar - zNear );
		result[15] = 1.0f;

		return result;
	}

	void Instance::registerLayer( Layer * layer )const
	{
		m_layers.push_back( layer );
	}

	void Instance::unregisterLayer( Layer * layer )const
	{
		auto it = std::find( m_layers.begin(), m_layers.end(), layer );

		if ( it != m_layers.end() )
		{
			m_layers.erase( it );
		}
	}

	bool Instance::onCopyToImageCommand( VkCommandBuffer cmd
		, VkBufferImageCopyArray const & copyInfo
		, VkBuffer src
		, VkImage dst )const
	{
		try
		{
			for ( auto & layer : m_layers )
			{
				layer->copyToImageCommand( cmd
					, copyInfo
					, src
					, dst );
			}
		}
		catch ( LayerException & exc )
		{
			std::cerr << exc.what() << std::endl;
			return true;
		}

		return false;
	}

#if VK_EXT_debug_utils

	void Instance::onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		try
		{
			for ( auto & layer : m_layers )
			{
				layer->submitDebugUtilsMessenger( messageSeverity
					, messageTypes
					, callbackData );
			}
		}
		catch ( LayerException & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
	}

#endif
#if VK_EXT_debug_report

	void Instance::onReportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		try
		{
			for ( auto & layer : m_layers )
			{
				layer->reportMessage( flags
					, objectType
					, object
					, location
					, messageCode
					, pLayerPrefix
					, pMessage );
			}
		}
		catch ( LayerException & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
	}

#endif

	VkPhysicalDeviceMemoryProperties const & Instance::getMemoryProperties()
	{
		static VkPhysicalDeviceMemoryProperties const memoryProperties = []()
		{
			VkPhysicalDeviceMemoryProperties result{};
			// Emulate one device local heap
			result.memoryHeaps[result.memoryHeapCount++] = { ~( 0ull ), VK_MEMORY_HEAP_DEVICE_LOCAL_BIT };
			// and one host visible heap
			result.memoryHeaps[result.memoryHeapCount++] = { ~( 0ull ), 0u };

			// Emulate all combinations of device local memory types
			// and all combinations of host visible memory types
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_PROTECTED_BIT, 0u };
#if VK_AMD_device_coherent_memory
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD, 0u };
#endif

			return result;
		}( );
		return memoryProperties;
	}

	VkPhysicalDeviceMemoryProperties2KHR const & Instance::getMemoryProperties2()
	{
		static VkPhysicalDeviceMemoryProperties2KHR const memoryProperties2 = []()
		{
			VkPhysicalDeviceMemoryProperties2KHR result{};
			result.memoryProperties = Instance::getMemoryProperties();
			return result;
		}( );
		return memoryProperties2;
	}

	void Instance::doInitialisePhysicalDevices()
	{
		m_physicalDevices.resize( 1u );
		allocate( m_physicalDevices[0]
			, nullptr
			, get( this ) );
	}
}
