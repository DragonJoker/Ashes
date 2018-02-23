#include "Core/GlContext.hpp"

#if RENDERLIB_WIN32
#	include "GlMswContext.hpp"
#elif RENDERLIB_XLIB
#	include "GlX11Context.hpp"
#endif

#include <iterator>

namespace gl_renderer
{
	Context::Context( renderer::ConnectionPtr && connection )
		: m_connection{ std::move( connection ) }
	{
	}

	ContextPtr Context::create( renderer::ConnectionPtr && connection )
	{
#if defined( _WIN32 )
		return std::make_unique< MswContext >( std::move( connection ) );
#elif defined( __linux__ )
		return std::make_unique< X11Context >( std::move( connection ) );
#endif
	}

	void Context::doInitialiseBaseInfo()
	{
		m_info.name = ( char const * )gl::GetString( GL_RENDERER );
		m_info.apiVersion = ( char const * )gl::GetString( GL_VERSION );
		auto const * res = ( char const * )gl::GetString( GL_EXTENSIONS );

		if ( res )
		{
			std::string extensions = res;
			std::istringstream stream{ extensions };
			std::copy( std::istream_iterator< std::string >( stream ),
				std::istream_iterator< std::string >(),
				std::back_inserter( m_extensions ) );
		}
	}

	void Context::doInitialiseInfo()
	{
		m_info.fillModeNonSolid = true;
		GLint range[2];
		glLogCall( gl::GetIntegerv, GL_ALIASED_LINE_WIDTH_RANGE, range );
		m_info.wideLines = ( range[1] > 1 );
		glLogCall( gl::GetIntegerv, GL_SMOOTH_LINE_WIDTH_RANGE, range );
		m_info.wideLines &= ( range[1] > 1 );
		glLogCall( gl::ClipControl, GL_UPPER_LEFT, GL_ZERO_TO_ONE );

		if ( doFindExtension( "GL_ARB_ES3_compatibility" )
			&& doFindExtension( "ARB_ES2_compatibility" )
			&& doFindExtension( "ARB_invalidate_subdata" )
			&& doFindExtension( "ARB_texture_storage" ) )
		{
			m_info.textureCompressionETC2 = true;
		}

		if ( doFindExtension( "GL_KHR_texture_compression_astc_ldr" ) )
		{
			m_info.textureCompressionASTC_LDR = true;
		}

		if ( doFindExtension( "GL_EXT_texture_compression_s3tc" )
			&& doFindExtension( "GL_EXT_texture_sRGB" ) )
		{
			m_info.textureCompressionBC = true;
		}
	}

	bool Context::doFindExtension( std::string const & name )
	{
		return m_extensions.end() != std::find( m_extensions.begin()
			, m_extensions.end()
			, name );
	}
}
