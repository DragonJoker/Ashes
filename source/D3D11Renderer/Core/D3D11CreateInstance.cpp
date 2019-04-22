#include "Core/D3D11CreateInstance.hpp"
#include "Core/D3D11Instance.hpp"

#include <Ashes/Core/Exception.hpp>

extern "C"
{
	D3D11Renderer_API ashes::Result createInstance( VkInstanceCreateInfo createInfo
		, ashes::Instance ** instance )
	{
		ashes::Result result = ashes::Result::eSuccess;

		try
		{
			assert( instance );
			*instance = new ashes::d3d11::Instance{ std::move( createInfo ) };
		}
		catch ( ashes::Exception & exc )
		{
			result = exc.getResult();
		}
		catch ( ... )
		{
			result = ashes::Result::eErrorRenderer;
		}

		return result;
	}

	D3D11Renderer_API ashes::Result enumerateVersion( uint32_t * version )
	{
		assert( version );
		*version = ashes::API_VERSION_1_0;
		return ashes::Result::eSuccess;
	}

	D3D11Renderer_API ashes::Result enumerateLayerProperties( uint32_t * count
		, VkLayerProperties * props )
	{
		*count = 0u;
		return ashes::Result::eSuccess;
	}

	D3D11Renderer_API ashes::Result enumerateExtensionProperties( char const * const layerName
		, uint32_t * count
		, VkExtensionProperties * props )
	{
		static ashes::VkExtensionPropertiesArray const extensions
		{
			{ ashes::KHR_SURFACE_EXTENSION_NAME, ashes::makeVersion( 1, 0, 0 ) },
			{ ashes::KHR_PLATFORM_SURFACE_EXTENSION_NAME, ashes::makeVersion( 1, 0, 0 ) },
			{ ashes::EXT_DEBUG_REPORT_EXTENSION_NAME, ashes::makeVersion( 1, 0, 0 ) },
		};

		*count = uint32_t( extensions.size() );

		if ( props )
		{
			for ( auto & extension : extensions )
			{
				*props = extension;
				++props;
			}
		}

		return ashes::Result::eSuccess;
	}

	D3D11Renderer_API char const * getShortName()
	{
		return "d3d11";
	}

	D3D11Renderer_API char const * getFullName()
	{
		return "Direct3D 11";
	}
}
