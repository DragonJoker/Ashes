#include "Core/GlWindow.hpp"

namespace ashes::gl4
{
#if ASHES_WIN32

	RenderWindow::RenderWindow() try
	{
		m_hInstance = ::GetModuleHandleA( nullptr );
		WNDCLASSA wc{};
		wc.lpfnWndProc = RenderWindow::WndProc;
		wc.hInstance = m_hInstance;
		wc.hbrBackground = ( HBRUSH )( COLOR_BACKGROUND );
		wc.lpszClassName = "DummyWindow";
		wc.style = CS_OWNDC;
		RegisterClassA( &wc );
		m_hWnd = ::CreateWindowExA( 0L
			, wc.lpszClassName
			, "DummyWindow"
			, WS_OVERLAPPEDWINDOW
			, 0
			, 0
			, 640
			, 480
			, nullptr
			, nullptr
			, wc.hInstance
			, nullptr );

		if ( !m_hWnd )
		{
			throw std::runtime_error{ "Couldn't create window" };
		}

		m_hDC = ::GetDC( m_hWnd );

		if ( doSelectFormat() )
		{
			m_hContext = wglCreateContext( m_hDC );

			if ( !m_hContext )
			{
				throw std::runtime_error{ "Couldn't create the context" };
			}

			wglMakeCurrent( m_hDC, m_hContext );
		}
		else
		{
			throw std::runtime_error{ "Couldn't find an appropriate pixel format" };
		}
	}
	catch ( std::exception & )
	{
		if ( m_hDC )
		{
			::ReleaseDC( m_hWnd, m_hDC );
		}

		if ( m_hWnd )
		{
			::DestroyWindow( m_hWnd );
		}

		throw;
	}

	RenderWindow::~RenderWindow()
	{
		wglMakeCurrent( nullptr, nullptr );
		wglDeleteContext( m_hContext );
		::ReleaseDC( m_hWnd, m_hDC );
		::DestroyWindow( m_hWnd );
	}

	VkWin32SurfaceCreateInfoKHR RenderWindow::getCreateInfo()const
	{
		return
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0u,
			m_hInstance,
			m_hWnd,
		};
	}

	bool RenderWindow::doSelectFormat()
	{
		bool result = false;
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.cColorBits = 24;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int pixelFormats = ::ChoosePixelFormat( m_hDC, &pfd );

		if ( pixelFormats )
		{
			result = ::SetPixelFormat( m_hDC, pixelFormats, &pfd ) != FALSE;
		}

		return result;
	}

	LRESULT CALLBACK RenderWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch ( message )
		{
		case WM_CREATE:
			break;

		default:
			return DefWindowProcA( hWnd, message, wParam, lParam );
		}

		return 0;
	}

#elif ASHES_XLIB

	RenderWindow::RenderWindow()
	{
		try
		{
			m_display = XOpenDisplay( NULL );

			if ( !m_display )
			{
				throw std::runtime_error{ "Couldn't open X Display" };
			}

			int attributes[] =
			{
				GLX_X_RENDERABLE, True,
				GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
				GLX_DOUBLEBUFFER, GL_TRUE,
				GLX_RED_SIZE, 1,
				GLX_GREEN_SIZE, 1,
				GLX_BLUE_SIZE, 1,
				0
			};

			int fbcount = 0;
			GLXFBConfig * config = glXChooseFBConfig( m_display, DefaultScreen( m_display ), attributes, &fbcount );
			int bestFbcIndex = -1;
			int worstFbcIndex = -1;
			int bestNumSamp = -1;
			int worstNumSamp = 999;

			for ( int i = 0; i < fbcount; ++i )
			{
				XVisualInfo * vi = glXGetVisualFromFBConfig( m_display, config[i] );

				if ( vi )
				{
					int sampleBuffers;
					int samples;
					glXGetFBConfigAttrib( m_display, config[i], GLX_SAMPLE_BUFFERS, &sampleBuffers );
					glXGetFBConfigAttrib( m_display, config[i], GLX_SAMPLES, &samples );

					if ( bestFbcIndex < 0 || sampleBuffers && samples > bestNumSamp )
					{
						bestFbcIndex = i;
						bestNumSamp = samples;
					}

					if ( worstFbcIndex < 0 || !sampleBuffers || samples < worstNumSamp )
					{
						worstFbcIndex = i;
						worstNumSamp = samples;
					}
				}

				XFree( vi );
			}

			if ( bestFbcIndex == -1 )
			{
				throw std::runtime_error{ "Couldn't find appropriate GLXFBConfig" };
			}

			m_fbConfig = config[bestFbcIndex];
			XVisualInfo * vi = glXGetVisualFromFBConfig( m_display, m_fbConfig );

			if ( !vi )
			{
				throw std::runtime_error{ "Couldn't find get XVisualInfo" };
			}

			Window root = RootWindow( m_display, vi->screen );
			m_map = XCreateColormap( m_display, root, vi->visual, AllocNone );

			if ( !m_map )
			{
				throw std::runtime_error{ "Couldn't create X Colormap" };
			}

			XSetWindowAttributes swa;
			swa.colormap = m_map;
			swa.background_pixmap = 0;
			swa.border_pixel = 0;
			swa.event_mask = StructureNotifyMask;
			m_xWindow = XCreateWindow( m_display
				, root
				, 0
				, 0
				, 640
				, 480
				, 0
				, vi->depth
				, InputOutput
				, vi->visual
				, CWBorderPixel | CWColormap | CWEventMask
				, &swa );

			if ( !m_xWindow )
			{
				throw std::runtime_error{ "Couldn't create X Window" };
			}

			XStoreName( m_display, m_xWindow, "DummyWindow" );
			XSync( m_display, False );

			int screen = DefaultScreen( m_display );
			int major{ 0 };
			int minor{ 0 };
			bool ok = glXQueryVersion( m_display, &major, &minor );

			m_glxContext = glXCreateContext( m_display, vi, nullptr, GL_TRUE );

			if ( !m_glxContext )
			{
				throw std::runtime_error{ "Could not create a rendering context." };
			}

			glXMakeCurrent( m_display, m_xWindow, m_glxContext );
			XFree( vi );
		}
		catch ( std::exception & p_exc )
		{
			if ( m_xWindow )
			{
				XDestroyWindow( m_display, m_xWindow );
				m_xWindow = 0;
			}

			if ( m_map )
			{
				XFreeColormap( m_display, m_map );
				m_map = 0;
			}

			if ( m_fbConfig )
			{
				XFree( m_fbConfig );
				m_fbConfig = nullptr;
			}

			if ( m_display )
			{
				XCloseDisplay( m_display );
				m_display = nullptr;
			}

			throw;
		}
	}

	RenderWindow::~RenderWindow()
	{
		if ( m_display )
		{
			if ( m_glxContext )
			{
				glXMakeCurrent( m_display, 0, nullptr );
				glXDestroyContext( m_display, m_glxContext );
			}

			if ( m_xWindow )
			{
				XDestroyWindow( m_display, m_xWindow );
			}

			if ( m_fbConfig )
			{
				XFree( m_fbConfig );
			}

			if ( m_map )
			{
				XFreeColormap( m_display, m_map );
			}

			// The next line produces a double free.
			//XCloseDisplay( m_display );
		}
	}

	VkXlibSurfaceCreateInfoKHR RenderWindow::getCreateInfo()const
	{
		return 
		{
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0u,
			m_display,
			m_xWindow,
		};
	}

#endif
}
