#include "Prerequisites.hpp"

#include <Renderer/Connection.hpp>
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

namespace vkapp
{
	renderer::Connection makeConnection( wxWindow * window
		, renderer::Renderer const & vulkan )
	{
#if defined( __WXMSW__ )

		renderer::Connection connection{ vulkan
			, wxGetInstance()
			, window->GetHandle() };

#else

		GtkWidget * widget{ static_cast< GtkWidget * >( window->GetHandle() ) };
		Window window{ None };
		Display * display{ nullptr };

		if ( widget )
		{
			auto gtkWindow{ gtk_widget_get_window( widget ) };

			if ( gtkWindow )
			{
				window = gdk_x11_drawable_get_xid( gtkWindow );
			}

			auto gtkDisplay{ gtk_widget_get_display( widget ) };

			if ( gtkDisplay )
			{
				display = gdk_x11_display_get_xdisplay( gtkDisplay );
			}
		}

		renderer::Connection connection{ vulkan
			, display
			, window };

#endif

		return connection;
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
}
