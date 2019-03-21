#include "Core/GlCreateInstance.hpp"
#include "Core/GlInstance.hpp"

#include <Ashes/Core/Exception.hpp>

extern "C"
{
	Gl3Renderer_API ashes::Result createInstance( ashes::InstanceCreateInfo createInfo
		, ashes::Instance ** instance )
	{
		ashes::Result result = ashes::Result::eSuccess;

		try
		{
			assert( instance );
			*instance = new gl_renderer::Instance{ std::move( createInfo ) };
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

	Gl3Renderer_API ashes::Result enumerateVersion( uint32_t * version )
	{
		assert( version );
		*version = ashes::API_VERSION_1_0;
		return ashes::Result::eSuccess;
	}

	Gl3Renderer_API ashes::Result enumerateLayerProperties( uint32_t * count
		, ashes::LayerProperties * props )
	{
		*count = 0u;
		return ashes::Result::eSuccess;
	}

	Gl3Renderer_API ashes::Result enumerateExtensionProperties( char const * const layerName
		, uint32_t * count
		, ashes::ExtensionProperties * props )
	{
		static ashes::ExtensionPropertiesArray const extensions
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

	Gl3Renderer_API char const * getShortName()
	{
		return "gl3";
	}

	Gl3Renderer_API char const * getFullName()
	{
		return "OpenGL 3.X";
	}
}
