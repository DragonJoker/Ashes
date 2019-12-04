/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlXcbWindow.hpp"

#if VK_USE_PLATFORM_XCB_KHR

#include <X11/Xlib-xcb.h>
#include <EGL/egl.h>
#include <sstream>
#include <iomanip>

namespace ashes::gl
{
	RenderWindow *& doGet()
	{
		static RenderWindow * result{ nullptr };
		return result;
	}

	RenderWindow::RenderWindow( int major
		, int minor
		, std::string const & name )
	{
		try
		{
			std::string fullName = "DummyWindow" + name;
			m_display = XOpenDisplay( nullptr );

			if ( !m_display )
			{
				throw std::runtime_error{ "Couldn't open display" };
			}

			m_connection = XGetXCBConnection( m_display );

			if ( !m_connection )
			{
				throw std::runtime_error{ "Couldn't get xcb connection from display" };
			}

			if ( xcb_connection_has_error( m_connection ) )
			{
				throw std::runtime_error{ "Errors occured in connecting to X server" };
			}

			xcb_setup_t const * setup = xcb_get_setup( m_connection );
			auto screen = xcb_setup_roots_iterator( setup ).data;
			assert( screen != nullptr );

			/* Create window */
			uint32_t valueList[] = { 0 };
			uint32_t valueMask = 0;

			m_window = xcb_generate_id( m_connection );
			auto createCookie = xcb_create_window_checked( m_connection
				, XCB_COPY_FROM_PARENT
				, m_window
				, screen->root
				, 0, 0
				, 150, 150
				, 0
				, XCB_WINDOW_CLASS_INPUT_OUTPUT
				, screen->root_visual
				, valueMask
				, valueList );
			auto error = xcb_request_check( m_connection, createCookie );

			if ( error )
			{
				std::stringstream stream;
				stream << "Failed to create XCB Window: " << int( error->error_code );
				throw std::runtime_error{ stream.str() };
			}

			m_context = std::make_unique< ContextEgl >( m_display
				, m_window
				, major
				, minor
				, EGL_NO_CONTEXT );
		}
		catch ( std::exception & p_exc )
		{
			doCleanup();
			throw;
		}
	}

	RenderWindow::~RenderWindow()
	{
		doCleanup();

		if ( m_display )
		{
			// XCloseDisplay( m_display );
			m_display = nullptr;
		}
	}

	void RenderWindow::create( int major, int minor )
	{
		assert( doGet() == nullptr );
		doGet() = new RenderWindow{ major, minor };
	}

	void RenderWindow::destroy()
	{
		delete doGet();
		doGet() = nullptr;
	}

	RenderWindow const & RenderWindow::get()
	{
		assert( doGet() != nullptr );
		return *doGet();
	}

	VkXcbSurfaceCreateInfoKHR RenderWindow::getCreateInfo()const
	{
		return 
		{
			VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0u,
			m_connection,
			m_window,
		};
	}

	void RenderWindow::doCleanup()
	{
		m_context.reset();

		if ( m_window )
		{
			xcb_destroy_window( m_connection, m_window );
			m_window = 0;
		}

		if ( m_connection )
		{
			xcb_disconnect( m_connection );
			m_connection = nullptr;
		}
	}
}

#endif
