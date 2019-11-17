/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlMswWindow.hpp"

#if _WIN32

namespace ashes::gl
{
	RenderWindow *& doGet()
	{
		static RenderWindow * result{ nullptr };
		return result;
	}

	RenderWindow::RenderWindow( int major, int minor ) try
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
		doCleanup();
		throw;
	}

	RenderWindow::~RenderWindow()
	{
		doCleanup();
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

	void RenderWindow::doCleanup()
	{
		if ( m_hContext )
		{
			wglMakeCurrent( nullptr, nullptr );
			wglDeleteContext( m_hContext );
		}
		
		if ( m_hDC )
		{
			::ReleaseDC( m_hWnd, m_hDC );
		}

		if ( m_hWnd )
		{
			::DestroyWindow( m_hWnd );
		}

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
}

#endif
