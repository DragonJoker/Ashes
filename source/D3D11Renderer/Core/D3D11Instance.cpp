#include "Core/D3D11Instance.hpp"

#include "Core/D3D11Surface.hpp"
#include "Core/D3D11DebugReportCallback.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11SwapChain.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <locale>

namespace d3d11_renderer
{
	namespace
	{
		D3D_FEATURE_LEVEL doGetSupportedFeatureLevel( IDXGIFactory * factory
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
				throw ashes::Exception{ ashes::Result::eErrorIncompatibleDriver, "Feature level retrieval" };
			}

			return result;
		}
	}

	ashes::PhysicalDeviceMemoryProperties const Instance::m_memoryProperties = []()
		{
			ashes::PhysicalDeviceMemoryProperties result;

			// Emulate one device local heap
			result.memoryHeaps.push_back(
				{
					~( 0ull ),
					0u | ashes::MemoryHeapFlag::eDeviceLocal
				} );
			// and one host visible heap
			result.memoryHeaps.push_back(
				{
					~( 0ull ),
					0u
				} );

			// Emulate all combinations of device local memory types
			// and all combinations of host visible memory types
			result.memoryTypes.push_back(
				{
					0u | ashes::MemoryPropertyFlag::eDeviceLocal,
					0u,
				} );
			result.memoryTypes.push_back(
				{
					0u | ashes::MemoryPropertyFlag::eHostVisible,
					1u,
				} );
			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eHostCoherent,
					1u,
				} );
			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eHostCached,
					1u,
				} );
			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eLazilyAllocated,
					1u,
				} );
			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eHostCoherent | ashes::MemoryPropertyFlag::eHostCached,
					1u,
				} );

			return result;
		}();

	Instance::Instance( ashes::InstanceCreateInfo createInfo )
		: ashes::Instance{ ashes::ClipDirection::eTopDown, "d3d11", std::move( createInfo ) }
	{
		doCreateDXGIFactory();
		doLoadAdapters();
		m_features.hasTexBufferRange = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasImageTexture = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasBaseInstance = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.hasStorageBuffers = m_maxFeatureLevel >= D3D_FEATURE_LEVEL_11_0;
		m_features.supportsPersistentMapping = false;
	}

	Instance::~Instance()
	{
		for ( auto & info : m_adapters )
		{
			safeRelease( info.adapter2 );
			safeRelease( info.adapter1 );
			safeRelease( info.adapter );
		}

		safeRelease( m_factory );
	}

	ashes::PhysicalDevicePtrArray Instance::enumeratePhysicalDevices()const
	{
		ashes::PhysicalDevicePtrArray result;

		for ( auto adapter : m_adapters )
		{
			result.emplace_back( std::make_unique< PhysicalDevice >( *this, std::move( adapter ) ) );
		}

		return result;
	}

	ashes::DevicePtr Instance::createDevice( ashes::SurfacePtr surface
		, ashes::DeviceCreateInfo createInfos )const
	{
		ashes::DevicePtr result;

		try
		{
			result = std::make_shared< Device >( *this
				, std::move( surface )
				, std::move( createInfos ) );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not initialise logical device:\n" } + exc.what() );
		}

		return result;
	}

	ashes::SurfacePtr Instance::createSurface( ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )const
	{
		return std::make_unique< Surface >( *this
			, gpu
			, std::move( handle ) );
	}

	ashes::DebugReportCallbackPtr Instance::createDebugReportCallback( ashes::DebugReportCallbackCreateInfo createInfo )const
	{
		return std::make_unique< DebugReportCallback >( *this
			, std::move( createInfo ) );
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

	bool Instance::onCopyToImageCommand( ashes::CommandBuffer const & cmd
		, ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Image const & dst )const
	{
		try
		{
			for ( auto & layer : m_layers )
			{
				layer->onCopyToImageCommand( cmd, copyInfo, src, dst );
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
				layer->onCheckHResultCommand( hresult, message );
			}
		}
		catch ( LayerException & exc )
		{
			std::cerr << exc.what() << std::endl;
			return true;
		}

		return false;
	}

	void Instance::doCreateDXGIFactory()
	{
		HRESULT hr = CreateDXGIFactory( __uuidof( IDXGIFactory )
			, reinterpret_cast< void ** >( &m_factory ) );

		if ( hr != S_OK )
		{
			throw std::runtime_error{ "Can't create Factory object" };
		}
	}

	void Instance::doLoadAdapters()
	{
		UINT index = 0;
		IDXGIAdapter * adapter;
		m_maxFeatureLevel = D3D_FEATURE_LEVEL_9_1;

		while ( m_factory->EnumAdapters( index, &adapter ) != DXGI_ERROR_NOT_FOUND )
		{
			IDXGIAdapter1 * adapter1{ nullptr };
			IDXGIAdapter2 * adapter2{ nullptr };
			auto hr = adapter->QueryInterface( __uuidof( IDXGIAdapter1 )
				, reinterpret_cast< void ** >( &adapter1 ) );

			if ( SUCCEEDED( hr ) )
			{
				hr = adapter1->QueryInterface( __uuidof( IDXGIAdapter2 )
					, reinterpret_cast< void ** >( &adapter2 ) );
			}

			auto featureLevel = doGetSupportedFeatureLevel( m_factory, adapter );
			m_maxFeatureLevel = std::max( featureLevel, m_maxFeatureLevel );
			m_adapters.push_back(
				{
					adapter,
					adapter1,
					adapter2,
					featureLevel
				} );
			++index;
		}
	}
}
