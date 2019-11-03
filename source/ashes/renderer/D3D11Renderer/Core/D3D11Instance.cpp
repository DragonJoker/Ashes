#include "Core/D3D11Instance.hpp"

#include "Core/D3D11Surface.hpp"
#include "Core/D3D11DebugReportCallback.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11SwapChain.hpp"

#include <ashes/common/Exception.hpp>
#include <ashes/ashes.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <locale>

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		IDXGIFactory * createDXGIFactory()
		{
			IDXGIFactory * result;
			HRESULT hr = CreateDXGIFactory( __uuidof( IDXGIFactory )
				, reinterpret_cast< void ** >( &result ) );

			if ( hr != S_OK )
			{
				throw std::runtime_error{ "Can't create Factory object" };
			}

			return result;
		}

		D3D_FEATURE_LEVEL getSupportedFeatureLevel( IDXGIFactory * factory
			, IDXGIAdapter * adapter )
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
			D3D_FEATURE_LEVEL result;
			auto hr = D3D11CreateDevice( adapter
				, D3D_DRIVER_TYPE_UNKNOWN
				, nullptr
				, 0u
				, requestedFeatureLevels.data()
				, UINT( requestedFeatureLevels.size() )
				, D3D11_SDK_VERSION
				, nullptr
				, &result
				, nullptr );

			if ( !SUCCEEDED( hr ) )
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
					hr = info.adapter1->QueryInterface( __uuidof( IDXGIAdapter2 )
						, reinterpret_cast< void ** >( &info.adapter2 ) );
				}

				if ( SUCCEEDED( adapter->EnumOutputs( 0, &info.output ) )
					&& info.output )
				{
					info.featureLevel = getSupportedFeatureLevel( factory, adapter );
					result.push_back( info );
				}

				++index;
			}

			return result;
		}

		D3D_FEATURE_LEVEL getMaxFeatureLevel( std::vector< AdapterInfo > const & adapters )
		{
			auto result = D3D_FEATURE_LEVEL_9_1;

			for ( auto adapter : adapters )
			{
				result = std::max( adapter.featureLevel, result );
			}

			return result;
		}
	}

	D3D_FEATURE_LEVEL getSupportedFeatureLevel()
	{
		D3D_FEATURE_LEVEL result = D3D_FEATURE_LEVEL_9_1;
		auto factory = createDXGIFactory();

		if ( factory )
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
		: m_flags{ createInfo.flags }
		, m_enabledLayerNames{ ashes::convert( CharPtrArray{ createInfo.ppEnabledLayerNames, createInfo.ppEnabledLayerNames + createInfo.enabledLayerCount } ) }
		, m_enabledExtensions{ ashes::convert( CharPtrArray{ createInfo.ppEnabledExtensionNames, createInfo.ppEnabledExtensionNames + createInfo.enabledExtensionCount } ) }
		, m_factory{ createDXGIFactory() }
	{
		doLoadAdapters();
		doInitialisePhysicalDevices();

		m_features.hasBufferRange = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasImageTexture = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasBaseInstance = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasStorageBuffers = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.supportsPersistentMapping = false;
	}

	Instance::~Instance()
	{
		for ( auto & physicalDevice : m_physicalDevices )
		{
			deallocate( physicalDevice, nullptr );
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

	bool Instance::onCheckHResultCommand( HRESULT hresult
		, std::string message )const
	{
		try
		{
			for ( auto & layer : m_layers )
			{
				layer->checkHResultCommand( hresult
					, message );
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

	void Instance::doLoadAdapters()
	{
		m_adapters = listAdapters( m_factory );
		m_maxFeatureLevel = getMaxFeatureLevel( m_adapters );
	}

	void Instance::doInitialisePhysicalDevices()
	{
		m_physicalDevices.resize( m_adapters.size() );
		uint32_t index = 0u;

		for ( auto adapter : m_adapters )
		{
			allocate( m_physicalDevices[index]
				, nullptr
				, get( this )
				, std::move( adapter ) );
			++index;
		}
	}
}
