#include "Core/D3D11Instance.hpp"

#include "Core/D3D11Surface.hpp"
#include "Core/D3D11DebugReportCallback.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11SwapChain.hpp"

#include <ashes/common/Exception.hpp>
#include <ashes/ashes.hpp>

#include "ashesd3d11_api.hpp"

#include <cmath>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <iomanip>
#include <iostream>
#include <locale>
#pragma warning( pop )

namespace ashes::D3D11_NAMESPACE
{
	namespace
	{
		inline VkPhysicalDeviceMemoryProperties const MemoryProperties = []()
		{
			VkPhysicalDeviceMemoryProperties result{};
			// Emulate one device local heap
			result.memoryHeaps[result.memoryHeapCount++] = { ~0ULL, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT };
			// and one host visible heap
			result.memoryHeaps[result.memoryHeapCount++] = { ~0ULL, 0u };

			// Emulate all combinations of device local memory types
			// and all combinations of host visible memory types
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, 1u };

			return result;
		}();

		inline VkPhysicalDeviceMemoryProperties2KHR const MemoryProperties2 = []()
		{
			VkPhysicalDeviceMemoryProperties2KHR result{};
			result.memoryProperties = Instance::getMemoryProperties();
			return result;
		}();

		IDXGIFactory * createDXGIFactory()
		{
			IDXGIFactory * result{};

#if Ashes_UWP || defined( Ashes_D3D11_XBox )
			if ( CreateDXGIFactory2( 0u
				, __uuidof( IDXGIFactory2 )
				, reinterpret_cast< void ** >( &result ) ) != S_OK )
#else
			if ( CreateDXGIFactory( __uuidof( IDXGIFactory )
				, reinterpret_cast< void ** >( &result ) ) != S_OK )
#endif
			{
				throw ashes::BaseException{ "Can't create Factory object" };
			}

			return result;
		}

		D3D_FEATURE_LEVEL getSupportedFeatureLevel( IDXGIAdapter * adapter )
		{
			static std::vector< D3D_FEATURE_LEVEL > const requestedFeatureLevels
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};
			D3D_FEATURE_LEVEL result{};

			if ( auto hr = D3D11CreateDevice( adapter
					, D3D_DRIVER_TYPE_UNKNOWN
					, nullptr
					, 0u
					, requestedFeatureLevels.data()
					, UINT( requestedFeatureLevels.size() )
					, D3D11_SDK_VERSION
					, nullptr
					, &result
					, nullptr );
				!SUCCEEDED( hr ) )
			{
				throw ashes::Exception{ VK_ERROR_INCOMPATIBLE_DRIVER, "Feature level retrieval" };
			}

			return result;
		}

		std::vector< AdapterInfo > listAdapters( IDXGIFactory * factory )
		{
			std::vector< AdapterInfo > result;
			UINT index = 0;
			IDXGIAdapter * adapter;

			while ( factory->EnumAdapters( index, &adapter ) != DXGI_ERROR_NOT_FOUND )
			{
				AdapterInfo info;
				info.adapter = adapter;
				auto hr = adapter->QueryInterface( __uuidof( IDXGIAdapter1 )
					, reinterpret_cast< void ** >( &info.adapter1 ) );

				if ( SUCCEEDED( hr ) )
				{
					( void )info.adapter1->QueryInterface( __uuidof( IDXGIAdapter2 )
						, reinterpret_cast< void ** >( &info.adapter2 ) );
				}

				( void )adapter->EnumOutputs( 0, &info.output );

				info.featureLevel = getSupportedFeatureLevel( adapter );
				result.push_back( info );

				++index;
			}

			return result;
		}

		D3D_FEATURE_LEVEL getMaxFeatureLevel( std::vector< AdapterInfo > const & adapters )
		{
			auto result = D3D_FEATURE_LEVEL_9_1;

			for ( auto const & adapter : adapters )
			{
				result = std::max( adapter.featureLevel, result );
			}

			return result;
		}

		void checkEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
		{
			auto & available = getSupportedInstanceExtensions( nullptr );

			for ( auto const & extension : extensions )
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

		bool hasEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
		{
			try
			{
				checkEnabledExtensions( extensions );
				return true;
			}
			catch ( ExtensionNotPresentException & )
			{
				return false;
			}
		}

		VkApplicationInfo getDefaultApplicationInfo()
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
	}

	D3D_FEATURE_LEVEL getSupportedFeatureLevel()
	{
		D3D_FEATURE_LEVEL result = D3D_FEATURE_LEVEL_9_1;

		if ( auto factory = createDXGIFactory() )
		{
			auto adapters = listAdapters( factory );
			result = getMaxFeatureLevel( adapters );

			for ( auto & adapter : adapters )
			{
				safeRelease( adapter.output );
				safeRelease( adapter.adapter2 );
				safeRelease( adapter.adapter1 );
				safeRelease( adapter.adapter );
			}

			safeRelease( factory );
		}

		return result;
	}

	Instance::Instance( VkInstanceCreateInfo const & createInfo )
		: m_flags{ createInfo.flags }
		, m_applicationInfo{ createInfo.pApplicationInfo ? *createInfo.pApplicationInfo : getDefaultApplicationInfo() }
		, m_enabledLayerNames{ ashes::convert( CharPtrArray{ createInfo.ppEnabledLayerNames, createInfo.ppEnabledLayerNames + createInfo.enabledLayerCount } ) }
		, m_enabledExtensions{ ashes::convert( CharPtrArray{ createInfo.ppEnabledExtensionNames, createInfo.ppEnabledExtensionNames + createInfo.enabledExtensionCount } ) }
		, m_factory{ createDXGIFactory() }
	{
		doLoadAdapters();
		doInitialisePhysicalDevices();

		m_features.hasTexBufferRange = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasImageTexture = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasBaseInstance = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasStorageBuffers = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.supportsPersistentMapping = false;

		checkEnabledExtensions( ashes::makeArrayView( createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount ) );
	}

	Instance::~Instance()noexcept
	{
		for ( auto & physicalDevice : m_physicalDevices )
		{
			deallocateNA( physicalDevice );
		}
		
		for ( auto & info : m_adapters )
		{
			safeRelease( info.output );
			safeRelease( info.adapter2 );
			safeRelease( info.adapter1 );
			safeRelease( info.adapter );
		}

		safeRelease( m_factory );
	}

	uint32_t Instance::getApiVersion()const
	{
		return m_applicationInfo.apiVersion;
	}

	bool Instance::hasExtension( std::string_view extension )const
	{
		char const * const version = extension.data();
		return hasEnabledExtensions( ashes::makeArrayView( &version, 1u ) );
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
		float const tanHalfFovy = tan( radiansFovY / float( 2 ) );

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

	void Instance::unregisterLayer( Layer * layer )const noexcept
	{
		auto it = std::find( m_layers.begin(), m_layers.end(), layer );

		if ( it != m_layers.end() )
		{
			m_layers.erase( it );
		}
	}

	void Instance::onCopyToImageCommand( VkCommandBuffer cmd
		, ArrayView< VkBufferImageCopy const > const & copyInfo
		, VkBuffer src
		, VkImage dst )const noexcept
	{
		for ( auto & layer : m_layers )
		{
			layer->copyToImageCommand( cmd
				, copyInfo
				, src
				, dst );
		}
	}

	void Instance::onCheckHResultCommand( HRESULT hresult
		, std::string const & message )const noexcept
	{
		for ( auto & layer : m_layers )
		{
			layer->checkHResultCommand( hresult
				, message );
		}
	}

#if VK_EXT_debug_utils

	void Instance::submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const noexcept
	{
		onSubmitDebugUtilsMessenger( messageSeverity
			, messageTypes
			, callbackData );
	}

	void Instance::onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const noexcept
	{
		for ( auto & layer : m_layers )
		{
			layer->submitDebugUtilsMessenger( messageSeverity
				, messageTypes
				, callbackData );
		}
	}

#endif
#if VK_EXT_debug_report

	void Instance::reportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )const noexcept
	{
		onReportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

	void Instance::onReportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )const noexcept
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

#endif

	void Instance::doLoadAdapters()
	{
		m_adapters = listAdapters( m_factory );
		m_maxFeatureLevel = getMaxFeatureLevel( m_adapters );
	}

	void Instance::doInitialisePhysicalDevices()
	{
		m_physicalDevices.resize( m_adapters.size() );
		uint32_t index = 0u;

		for ( auto const & adapter : m_adapters )
		{
			allocateNA( m_physicalDevices[index]
				, get( this )
				, adapter );
			++index;
		}
	}

	VkPhysicalDeviceMemoryProperties const & Instance::getMemoryProperties()noexcept
	{
		return MemoryProperties;
	}

	VkPhysicalDeviceMemoryProperties2KHR const & Instance::getMemoryProperties2()noexcept
	{
		return MemoryProperties2;
	}
}
