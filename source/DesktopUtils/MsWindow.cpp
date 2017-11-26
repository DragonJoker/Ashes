#include "MsWindow.h"

#include <RenderLib/RenderLibPrerequisites.h>

#include <Renderer/Connection.hpp>
#include <Renderer/RenderingResources.hpp>

#include <VkLib/FlagCombination.hpp>

#include <Windows.h>
#include <windowsx.h>


namespace utils
{
	namespace
	{
		std::string doGetExecutableDir()
		{
			std::string ret;
			ret.resize( 1024 );
			uint32_t count{ 1023u };
			count = ::GetModuleFileNameA( nullptr, &ret[0], count );
			ret.resize( count + 1 );
			ret = ret.substr( 0, ret.find_last_of( R"(\)" ) );
			return ret;
		}
	}

	std::map< HWND, MsWindow * > MsWindow::sm_instances;

	MsWindow::MsWindow()
	{
#if !defined( NDEBUG )
#	if defined( _WIN32 )

		if ( ::AllocConsole() )
		{
			m_allocated = true;
			FILE * dump;
			freopen_s( &dump, "conout$", "w", stdout );
			freopen_s( &dump, "conout$", "w", stderr );
		}
		else
		{
			DWORD lastError = ::GetLastError();

			if ( lastError == ERROR_ACCESS_DENIED )
			{
				FILE * dump;
				freopen_s( &dump, "conout$", "w", stdout );
				freopen_s( &dump, "conout$", "w", stderr );
			}
		}

#	endif
#endif

		::SetCurrentDirectoryA( doGetExecutableDir().c_str() );
	}
	
	MsWindow::~MsWindow()
	{
#if !defined( NDEBUG )
#	if defined( _WIN32 )

		if ( m_allocated )
		{
			::FreeConsole();
		}

#	endif
#endif
	}

	void MsWindow::create( HINSTANCE instance
		, std::string const & className
		, int iconResourceID
		, int smallIconResourceID
		, std::string const & title )
	{
		doRegisterClass( instance
			, className
			, iconResourceID
			, smallIconResourceID );

		m_hwnd = ::CreateWindowExA( 0u
			, className.c_str()
			, title.c_str()
			, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
			, CW_USEDEFAULT
			, 0
			, CW_USEDEFAULT
			, 0
			, nullptr
			, nullptr
			, instance
			, this );

		if ( m_hwnd )
		{
			::ShowWindow( m_hwnd, TRUE );
			::UpdateWindow( m_hwnd );
			::SetTimer( m_hwnd, 1, 17, nullptr );
			m_timer = 1;
			doRegisterInstance( this );
			doCreate();
		}
		else
		{
			throw::std::runtime_error{ "Couldn't create the window." };
		}
	}

	void MsWindow::doSwapBuffers()
	{
		::SwapBuffers( m_hdc );
	}

	void MsWindow::doCreate()
	{
		m_hdc = ::GetDC( m_hwnd );

		if ( !m_hdc )
		{
			std::cout << "HDC creation failed." << std::endl;
		}
		else
		{
			if ( doPrepareDC( m_hdc ) )
			{
				m_renderer = std::make_unique< renderer::Renderer >();
				RECT rect;
				::GetClientRect( m_hwnd, &rect );
				m_size.x = rect.right - rect.left;
				m_size.y = rect.bottom - rect.top;
				m_device = m_renderer->createDevice( doCreateConnection() );

				if ( m_device )
				{
					onCreate();
				}
			}
		}
	}

	void MsWindow::doDestroy()
	{
		if ( m_context != INVALID_HANDLE_VALUE )
		{
			onDestroy();
			m_renderer.reset();
			::ReleaseDC( m_hwnd, m_hdc );
		}

		m_hdc = nullptr;
		m_context = nullptr;
	}

	void MsWindow::doMinimise()
	{
		::KillTimer( m_hwnd, 1 );
		m_timer = -1;
		onMinimise();
		m_minimised = true;
	}

	void MsWindow::doRestore( utils::IVec2 const & size )
	{
		if ( m_minimised )
		{
			m_size = size;
			onRestore( size );
			::SetTimer( m_hwnd, 1, 17, nullptr );
			m_timer = 1;
		}
		else if ( m_size != size )
		{
			m_size = size;
			onResize( m_size );
		}
	}

	bool MsWindow::doPrepareDC( HDC hdc )
	{
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.cColorBits = 32;

		auto count = ::ChoosePixelFormat( hdc, &pfd );

		if ( !count )
		{
			std::cerr << "Pixel format selection failed." << std::endl;
			return false;
		}

		if ( ::SetPixelFormat( hdc, count, &pfd ) == FALSE )
		{
			std::cerr << "Pixel format selection failed." << std::endl;
			return false;
		}

		return true;
	}

	renderer::Connection MsWindow::doCreateConnection()
	{
		return renderer::Connection{ *m_renderer
			, ::GetModuleHandle( nullptr )
			, m_hwnd };
	}

	void MsWindow::doRegisterClass( HINSTANCE hInstance
		, std::string const & className
		, int iconResourceID
		, int smallIconResourceID )
	{
		WNDCLASSEXA wcex;

		wcex.cbSize = sizeof( WNDCLASSEX );

		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = MsWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = ::LoadIconA( hInstance, MAKEINTRESOURCEA( iconResourceID ) );
		wcex.hCursor = ::LoadCursorA( nullptr, IDC_ARROW );
		wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = className.c_str();
		wcex.hIconSm = ::LoadIconA( wcex.hInstance, MAKEINTRESOURCEA( smallIconResourceID ) );

		::RegisterClassExA( &wcex );
	}

	longptr_t MsWindow::processMessage( uint32_t message
		, size_t wParam
		, longptr_t lParam )
	{
		static utils::Clock::time_point time;
		static constexpr utils::Milliseconds clickTimeout{ 200 };

		switch ( message )
		{
		case WM_PAINT:
			{
				PAINTSTRUCT paint;
				HDC hdc = ::BeginPaint( m_hwnd, &paint );
				onDraw();
				::EndPaint( m_hwnd, &paint );
			}
			break;

		case WM_SIZE:
			{
				int w = LOWORD( lParam );
				int h = HIWORD( lParam );

				switch ( wParam )
				{
				case SIZE_MINIMIZED:
					doMinimise();
					break;

				case SIZE_RESTORED:
				case SIZE_MAXIMIZED:
					doRestore( { w, h } );
				}
			}
			break;

		case WM_MOUSEMOVE:
			{
				int w = GET_X_LPARAM( lParam );
				int h = GET_Y_LPARAM( lParam );
				bool ldown = ( wParam & MK_LBUTTON ) == MK_LBUTTON;
				bool mdown = ( wParam & MK_MBUTTON ) == MK_MBUTTON;
				bool rdown = ( wParam & MK_RBUTTON ) == MK_RBUTTON;
				onMouseMove( { { w, h }, ldown, mdown, rdown, 0 } );
			}
			break;

		case WM_LBUTTONDOWN:
			{
				int w = GET_X_LPARAM( lParam );
				int h = GET_Y_LPARAM( lParam );
				bool ldown = ( wParam & MK_LBUTTON ) == MK_LBUTTON;
				bool mdown = ( wParam & MK_MBUTTON ) == MK_MBUTTON;
				bool rdown = ( wParam & MK_RBUTTON ) == MK_RBUTTON;
				onLButtonDown( { { w, h }, ldown, mdown, rdown, 0 } );
				time = utils::Clock::now();
			}
			break;

		case WM_LBUTTONUP:
			{
				auto diff = std::chrono::duration_cast< utils::Milliseconds >( utils::Clock::now() - time );

				if ( diff < clickTimeout )
				{
					int w = GET_X_LPARAM( lParam );
					int h = GET_Y_LPARAM( lParam );
					bool ldown = ( wParam & MK_LBUTTON ) == MK_LBUTTON;
					bool mdown = ( wParam & MK_MBUTTON ) == MK_MBUTTON;
					bool rdown = ( wParam & MK_RBUTTON ) == MK_RBUTTON;
					onLButtonClick( { { w, h }, ldown, mdown, rdown, 0 } );
				}
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				int w = GET_X_LPARAM( lParam );
				int h = GET_Y_LPARAM( lParam );
				bool ldown = ( wParam & MK_LBUTTON ) == MK_LBUTTON;
				bool mdown = ( wParam & MK_MBUTTON ) == MK_MBUTTON;
				bool rdown = ( wParam & MK_RBUTTON ) == MK_RBUTTON;
				onLButtonDblClick( { { w, h }, ldown, mdown, rdown, 0 } );
			}
			break;

		case WM_MOUSEWHEEL:
			{
				int w = GET_X_LPARAM( lParam );
				int h = GET_Y_LPARAM( lParam );
				bool ldown = ( wParam & MK_LBUTTON ) == MK_LBUTTON;
				bool mdown = ( wParam & MK_MBUTTON ) == MK_MBUTTON;
				bool rdown = ( wParam & MK_RBUTTON ) == MK_RBUTTON;
				auto delta = GET_WHEEL_DELTA_WPARAM( wParam );
				onMouseWheel( { { w, h }, ldown, mdown, rdown, delta } );
			}
			break;

		case WM_TIMER:
			{
				auto hdc = ::GetDC( m_hwnd );
				onDraw();
				::ReleaseDC( m_hwnd, hdc );
			}
			break;

		case WM_DESTROY:
			{
				doDestroy();
				doUnregisterInstance( this );
				::KillTimer( m_hwnd, 1 );
				::PostQuitMessage( 0 );
			}
			break;

		default:
			return ::DefWindowProcA( m_hwnd, message, wParam, lParam );
		}

		return 0;
	}

	longptr_t CALLBACK MsWindow::WndProc( HWND hWnd
		, uint32_t message
		, size_t wParam
		, longptr_t lParam )
	{
		auto window = doGetInstance( hWnd );

		if ( !window )
		{
			return ::DefWindowProcA( hWnd, message, wParam, lParam );
		}

		return window->processMessage( message, wParam, lParam );
	}

	void MsWindow::doRegisterInstance( MsWindow * window )
	{
		sm_instances.emplace( window->m_hwnd, window );
	}

	void MsWindow::doUnregisterInstance( MsWindow * window )
	{
		auto it = sm_instances.find( window->m_hwnd );

		if ( it != sm_instances.end() )
		{
			sm_instances.erase( it );
		}
	}

	MsWindow * MsWindow::doGetInstance( HWND hWnd )
	{
		MsWindow * result{ nullptr };
		auto it = sm_instances.find( hWnd );

		if ( it != sm_instances.end() )
		{
			result = it->second;
		}

		return result;
	}
}
