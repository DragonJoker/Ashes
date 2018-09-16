#include "Core/D3D11Renderer.hpp"

#include "Core/D3D11Connection.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11SwapChain.hpp"

#include <cmath>
#include <iomanip>
#include <locale>

namespace d3d11_renderer
{
	Renderer::Renderer( Configuration const & configuration )
		: ashes::Renderer{ ashes::ClipDirection::eTopDown, "d3d11", configuration }
	{
		m_features.hasTexBufferRange = true;
		m_features.hasImageTexture = true;
		m_features.hasBaseInstance = true;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = true;
		m_features.hasStorageBuffers = true;
		doCreateDXGIFactory();
		doLoadAdapters();
	}

	Renderer::~Renderer()
	{
		safeRelease( m_factory );
	}

	ashes::DevicePtr Renderer::createDevice( ashes::ConnectionPtr && connection )const
	{
		ashes::DevicePtr result;

		try
		{
			result = std::make_shared< Device >( *this, std::move( connection ) );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not initialise logical device:\n" } + exc.what() );
		}

		return result;
	}

	ashes::ConnectionPtr Renderer::createConnection( uint32_t deviceIndex
		, ashes::WindowHandle && handle )const
	{
		return std::make_unique< Connection >( *this
			, deviceIndex
			, std::move( handle ) );
	}

	std::array< float, 16 > Renderer::frustum( float left
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

	std::array< float, 16 > Renderer::perspective( float radiansFovY
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

		//float const tanHalfFovy = tan( radiansFovY / float( 2 ) );

		//std::array< float, 16 > result{ 0.0f };
		//result[0] = float( 1 ) / ( aspect * tanHalfFovy );
		//result[5] = float( 1 ) / ( tanHalfFovy );
		//result[11] = -float( 1 );
		//result[10] = zFar / ( zNear - zFar );
		//result[14] = -( zFar * zNear ) / ( zFar - zNear );

		//return result;
	}

	std::array< float, 16 > Renderer::ortho( float left
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

	void Renderer::doCreateDXGIFactory()
	{
		HRESULT hr = CreateDXGIFactory( __uuidof( IDXGIFactory )
			, reinterpret_cast< void ** >( &m_factory ) );

		if ( hr != S_OK )
		{
			throw std::runtime_error{ "Can't create Factory object" };
		}
	}

	void Renderer::doLoadAdapters()
	{
		UINT index = 0;
		IDXGIAdapter * adapter;

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

			m_gpus.emplace_back( std::make_unique< PhysicalDevice >( *this, adapter, adapter1, adapter2 ) );
			++index;
		}
	}
}
