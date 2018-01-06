#include "Prerequisites.hpp"

#include <Renderer/Connection.hpp>
#include <Renderer/PlatformWindowHandle.hpp>
#include <Renderer/Renderer.hpp>

#if defined( __WXGTK__ )
#	include <gdk/gdkx.h>
#	include <gtk/gtk.h>
#elif defined( __WXMSW__ )
#	include <wx/msw/private.h>
#endif

#if defined( near )
#	undef near
#	undef far
#endif

#include <fstream>

namespace common
{
#if defined( _WIN32 )
	static char constexpr PathSeparator = '\\';
#else
	static char constexpr PathSeparator = '/';
#endif

	renderer::ConnectionPtr makeConnection( wxWindow * window
		, renderer::Renderer const & renderer )
	{
#if defined( __WXMSW__ )

		auto handle = renderer::WindowHandle{ std::make_unique< renderer::IMswWindowHandle >( wxGetInstance()
			, window->GetHandle() ) };
		return renderer.createConnection( 0u
			, std::move( handle ) );

#else

		GtkWidget * widget{ static_cast< GtkWidget * >( window->GetHandle() ) };
		Window xwindow{ None };
		Display * xdisplay{ nullptr };

		if ( widget )
		{
			auto gtkWindow{ gtk_widget_get_window( widget ) };

			if ( gtkWindow )
			{
				xwindow = gdk_x11_drawable_get_xid( gtkWindow );
			}

			auto gtkDisplay{ gtk_widget_get_display( widget ) };

			if ( gtkDisplay )
			{
				xdisplay = gdk_x11_display_get_xdisplay( gtkDisplay );
			}
		}

		return renderer.createConnection( 0u
			, renderer::WindowHandle{ std::make_unique< renderer::IXWindowHandle >( xwindow
				, xdisplay ) } );

#endif
	}

	std::array< float, 16 > getOrthographicProjection( float left
		, float right
		, float top
		, float bottom
		, float near
		, float far )
	{
		return
		{
			2.0f / ( right - left ),
			0.0f,
			0.0f,
			0.0f,

			0.0f,
			2.0f / ( bottom - top ),
			0.0f,
			0.0f,

			0.0f,
			0.0f,
			-2.0f / ( far - near ),
			0.0f,

			-( right + left ) / ( right - left ),
			-( bottom + top ) / ( bottom - top ),
			-( far + near ) / ( far - near ),
			1.0f
		};
	}

	std::string dumpTextFile( std::string const & path )
	{
		std::ifstream file( path );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		std::string result( static_cast< size_t >( end - begin ) + 1u, '\0' );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( &result[0] ), end - begin );

		return result;
	}

	renderer::ByteArray dumpBinaryFile( std::string const & path )
	{
		std::ifstream file( path, std::ios::binary );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		renderer::ByteArray result( static_cast< size_t >( end - begin ) );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( result.data() ), end - begin );

		return result;
	}

	renderer::UInt32Array dumpSpvFile( std::string const & path )
	{
		std::ifstream file( path, std::ios::binary );

		if ( file.fail() )
		{
			throw std::runtime_error{ "Could not open file " + path };
		}

		auto begin = file.tellg();
		file.seekg( 0, std::ios::end );
		auto end = file.tellg();

		renderer::UInt32Array result( static_cast< size_t >( end - begin ) / sizeof( uint32_t ) );
		file.seekg( 0, std::ios::beg );
		file.read( reinterpret_cast< char * >( result.data() ), end - begin );

		return result;
	}

	std::string getExecutableDirectory()
	{
		std::string pathReturn;

#if defined( _WIN32 )

		char path[FILENAME_MAX];
		DWORD result = ::GetModuleFileNameA( nullptr
			, path
			, sizeof( path ) );

		if ( result != 0 )
		{
			pathReturn = path;
		}

#elif defined( __linux__ )

		char path[FILENAME_MAX];
		char buffer[32];
		sprintf( buffer, "/proc/%d/exe", getpid() );
		int bytes = std::min< std::size_t >( readlink( buffer
			, path
			, sizeof( path ) )
			, sizeof( path ) - 1 );

		if ( bytes > 0 )
		{
			path[bytes] = '\0';
			pathReturn = path;
		}

#else

#	error "Unsupported platform"

#endif

		pathReturn = getPath( pathReturn );
		return pathReturn;
	}

	std::string getPath( std::string const & path )
	{
		return path.substr( 0, path.find_last_of( PathSeparator ) );
	}
}

std::string operator/( std::string const & lhs, std::string const & rhs )
{
	return lhs + common::PathSeparator + rhs;
}
