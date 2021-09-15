#include "Prerequisites.hpp"

#include <ashespp/Core/PlatformWindowHandle.hpp>

#if defined( __WXGTK__ )
#	include <gdk/gdkx.h>
#	include <gtk/gtk.h>
#	include <GL/glx.h>
#elif defined( __WXMSW__ )
#	include <wx/msw/private.h>
#elif defined( __WXOSX_COCOA__ )
#	include "MetalLayer.h"
#endif

#if defined( near )
#	undef near
#	undef far
#endif

#include <fstream>
#include <iomanip>

namespace common
{
	ashes::WindowHandle makeWindowHandle( wxWindow const & window )
	{
#if defined( __WXMSW__ )

		return ashes::WindowHandle{ std::make_unique< ashes::IMswWindowHandle >( wxGetInstance()
			, window.GetHandle() ) };

#elif defined( __WXGTK__ )

		auto gtkWidget = static_cast< GtkWidget * >( window.GetHandle() );
		auto gdkWindow = gtk_widget_get_window( gtkWidget );
		GLXDrawable drawable = 0;
		Display * display = nullptr;

		if ( gtkWidget && gdkWindow )
		{
			drawable = GDK_WINDOW_XID( gdkWindow );
			GdkDisplay * gtkDisplay = gtk_widget_get_display( gtkWidget );

			if ( gtkDisplay )
			{
				display = gdk_x11_display_get_xdisplay( gtkDisplay );
			}
		}

		return ashes::WindowHandle( std::make_unique< ashes::IXWindowHandle >( drawable, display ) );

#elif defined( __WXOSX_COCOA__ )

		auto handle = window.GetHandle();
		makeViewMetalCompatible( handle );
		return ashes::WindowHandle{ std::make_unique< ashes::IMacOsWindowHandle >( handle ) };

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

	wxString makeName( int index
		, wxString const & name )
	{
		std::stringstream stream;
		stream << std::setfill( '0' ) << std::setw( 2 ) << index;
		return wxString( stream.str() ) + wxT( "-" ) + name;
	}
}
