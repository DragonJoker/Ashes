#include "Prerequisites.hpp"

#include <Ashes/Core/Connection.hpp>
#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <Ashes/Core/Renderer.hpp>
#include <Ashes/Image/TextureView.hpp>

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
	ashes::ConnectionPtr makeConnection( wxWindow * window
		, ashes::Renderer const & renderer )
	{
#if defined( __WXMSW__ )

		auto handle = ashes::WindowHandle{ std::make_unique< ashes::IMswWindowHandle >( wxGetInstance()
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
			, ashes::WindowHandle{ std::make_unique< ashes::IXWindowHandle >( xwindow
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

	std::vector< ashes::Format > getFormats( ashes::TextureViewCRefArray const & views )
	{
		std::vector< ashes::Format > result;

		for ( auto view : views )
		{
			result.push_back( view.get().getFormat() );
		}

		return result;
	}
}