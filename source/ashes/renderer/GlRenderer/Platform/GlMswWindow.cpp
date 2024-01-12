/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlMswWindow.hpp"

#if _WIN32

#include <atomic>

namespace ashes::gl
{
	namespace win
	{
		LRESULT CALLBACK DummyWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
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

		ATOM registerClass( HINSTANCE hInstance
			, std::string const & name )
		{
			static std::atomic_uint32_t id{};
			std::string className = "Window" + name + "Class" + std::to_string( id++ );
			WNDCLASSEXA wc{};
			wc.cbSize = sizeof( WNDCLASSEXA );
			wc.lpfnWndProc = DummyWndProc;
			wc.hInstance = hInstance;
			wc.hbrBackground = ( HBRUSH )( COLOR_BACKGROUND );
			wc.lpszClassName = className.c_str();
			wc.style = CS_OWNDC;
			wc.hIcon = nullptr;
			auto registered = RegisterClassExA( &wc );

			if ( !registered )
			{
				throw ashes::BaseException{ "Couldn't register the window class" };
			}

			return registered;
		}

		HWND createWindow( HINSTANCE hInstance
			, ATOM classId
			, std::string const & name
			, DWORD dwFlags
			, VkExtent2D const & extent )
		{
			static std::atomic_uint32_t id{};
			auto windowName = "Window" + name + std::to_string( id++ );
			auto hWnd = ::CreateWindowExA( 0L
				, LPCSTR( WPARAM( classId ) )
				, windowName.c_str()
				, dwFlags
				, 0
				, 0
				, int( extent.width )
				, int( extent.height )
				, nullptr
				, nullptr
				, hInstance
				, nullptr );

			if ( !hWnd )
			{
				throw ashes::BaseException{ "Couldn't create window" };
			}

			return hWnd;
		}
	}

	HGLRC createContext( HDC hDC
		, PIXELFORMATDESCRIPTOR & pfd )
	{
		pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.cColorBits = 24;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 0;
		pfd.cStencilBits = 0;

		int pixelFormat = ::ChoosePixelFormat( hDC, &pfd );

		if ( !pixelFormat )
		{
			throw ashes::BaseException{ "Couldn't find matching pixel formats" };
		}

		if ( ::SetPixelFormat( hDC, pixelFormat, &pfd ) == FALSE )
		{
			throw ashes::BaseException{ "Couldn't set matching pixel format" };
		}

		auto context = wglCreateContext( hDC );

		if ( !context )
		{
			throw ashes::BaseException{ "Couldn't create OpenGL context" };
		}

		return context;
	}

	RenderWindow::RenderWindow( [[maybe_unused]] int major
		, [[maybe_unused]] int minor
		, std::string const & name )
	{
		try
		{
			m_hInstance = ::GetModuleHandleA( nullptr );
			m_class = win::registerClass( m_hInstance, name );
			m_hWnd = win::createWindow( m_hInstance, m_class, name, WS_OVERLAPPEDWINDOW, { 640u, 480u } );
			m_hDC = ::GetDC( m_hWnd );
			m_hContext = createContext( m_hDC, m_pfd.pfd );
			wglMakeCurrent( m_hDC, m_hContext );
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}
	}

	RenderWindow::~RenderWindow()noexcept
	{
		wglMakeCurrent( nullptr, nullptr );
		doCleanup();
	}

	void RenderWindow::doCleanup()noexcept
	{
		if ( m_hContext )
		{
			wglDeleteContext( m_hContext );
			m_hContext = nullptr;
		}

		if ( m_hDC )
		{
			::ReleaseDC( m_hWnd, m_hDC );
			m_hDC = nullptr;
		}

		if ( m_hWnd )
		{
			::DestroyWindow( m_hWnd );
			m_hWnd = nullptr;
		}

		if ( m_class )
		{
			::UnregisterClassA( LPCSTR( WPARAM( m_class ) ), m_hInstance );
			m_class = 0;
		}

	}

	VkWin32SurfaceCreateInfoKHR RenderWindow::getCreateInfo()const
	{
		return
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			&m_pfd,
			0u,
			m_hInstance,
			m_hWnd,
		};
	}
}

#endif
