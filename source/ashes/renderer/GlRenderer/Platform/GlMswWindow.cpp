/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlMswWindow.hpp"

#if _WIN32

namespace ashes::gl
{
	namespace
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
			std::string className = "Window" + name + "Class";
			WNDCLASSEXA wc{};
			wc.cbSize = sizeof( WNDCLASSEXA );
			wc.lpfnWndProc = DummyWndProc;
			wc.hInstance = hInstance;
			wc.hbrBackground = ( HBRUSH )( COLOR_BACKGROUND );
			wc.lpszClassName = className.c_str();
			wc.style = CS_OWNDC;
			wc.hIcon = nullptr;
			ATOM registered = RegisterClassExA( &wc );

			if ( !registered )
			{
				throw std::runtime_error{ "Couldn't register the window class" };
			}

			return registered;
		}

		HWND createWindow( HINSTANCE hInstance
			, ATOM classId, std::string const & name )
		{
			auto windowName = "Window" + name;
			auto hWnd = ::CreateWindowExA( 0L
				, LPCSTR( WPARAM( classId ) )
				, windowName.c_str()
				, WS_OVERLAPPEDWINDOW
				, 0
				, 0
				, 640
				, 480
				, nullptr
				, nullptr
				, hInstance
				, nullptr );

			if ( !hWnd )
			{
				throw std::runtime_error{ "Couldn't create window" };
			}

			return hWnd;
		}

		HGLRC createContext( HDC hDC, int reqMajor, int reqMinor )
		{
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

			int pixelFormat = ::ChoosePixelFormat( hDC, &pfd );

			if ( !pixelFormat )
			{
				throw std::runtime_error{ "Couldn't find matching pixel formats" };
			}

			if ( ::SetPixelFormat( hDC, pixelFormat, &pfd ) == FALSE )
			{
				throw std::runtime_error{ "Couldn't set matching pixel format" };
			}

			auto context = wglCreateContext( hDC );

			if ( !context )
			{
				throw std::runtime_error{ "Couldn't create OpenGL context" };
			}

			return context;
		}
	}

	RenderWindow::RenderWindow( int major
		, int minor
		, std::string const & name ) try
		: m_hInstance{ ::GetModuleHandleA( nullptr ) }
		, m_class{ registerClass( m_hInstance, name ) }
		, m_hWnd{ createWindow( m_hInstance, m_class, name ) }
		, m_hDC{ ::GetDC( m_hWnd ) }
		, m_hContext{ createContext( m_hDC, major, minor ) }
	{
		wglMakeCurrent( m_hDC, m_hContext );
	}
	catch ( std::exception & )
	{
		doCleanup();
		throw;
	}

	RenderWindow::~RenderWindow()
	{
		wglMakeCurrent( nullptr, nullptr );
		doCleanup();
	}

	void RenderWindow::doCleanup()
	{
		if ( m_hContext )
		{
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

		if ( m_class )
		{
			::UnregisterClassA( LPCSTR( WPARAM( m_class ) ), m_hInstance );
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
}

#endif
