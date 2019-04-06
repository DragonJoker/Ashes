#include "Core/GlInstance.hpp"

#include "Core/GlSurface.hpp"
#include "Core/GlContext.hpp"
#include "Core/GlDebugReportCallback.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

#if ASHES_WIN32
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#elif ASHES_XLIB
#	include <X11/X.h>
#	include <X11/Xlib.h>
#	include <GL/glx.h>
#endif

#include "ashesgl4_api.hpp"

#undef CreateWindow

namespace ashes::gl4
{
#if defined( _WIN32 )

	class RenderWindow
	{
	public:
		RenderWindow() try
		{
			m_hInstance = ::GetModuleHandle( nullptr );
			MSG msg{};
			WNDCLASSA wc{};
			wc.lpfnWndProc = RenderWindow::WndProc;
			wc.hInstance = m_hInstance;
			wc.hbrBackground = ( HBRUSH )( COLOR_BACKGROUND );
			wc.lpszClassName = "DummyWindow";
			wc.style = CS_OWNDC;
			RegisterClassA( &wc );
			m_hWnd = CreateWindowA( wc.lpszClassName
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

		~RenderWindow()
		{
			wglMakeCurrent( nullptr, nullptr );
			wglDeleteContext( m_hContext );
			::ReleaseDC( m_hWnd, m_hDC );
			::DestroyWindow( m_hWnd );
		}

		inline VkWin32SurfaceCreateInfoKHR getCreateInfo()const
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

	private:
		bool doSelectFormat()
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

		static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
		{
			switch ( message )
			{
			case WM_CREATE:
				break;

			default:
				return DefWindowProc( hWnd, message, wParam, lParam );
			}

			return 0;
		}

	private:
		HWND m_hWnd{ nullptr };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
		HINSTANCE m_hInstance{ nullptr };
	};

#elif defined( __linux__ )

	class RenderWindow
	{
	public:
		RenderWindow()
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
				m_xWindow = XCreateWindow( m_display, root, 0, 0, 640, 480, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa );

				if ( !m_xWindow )
				{
					throw std::runtime_error{ "Couldn't create X Window" };
				}

				XStoreName( m_display, m_xWindow, "DummyWindow" );
				XMapWindow( m_display, m_xWindow );
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
				}

				if ( m_map )
				{
					XFreeColormap( m_display, m_map );
				}

				if ( m_display )
				{
					XCloseDisplay( m_display );
				}

				throw;
			}
		}

		~RenderWindow()
		{
			glXMakeCurrent( m_display, 0, nullptr );
			glXDestroyContext( m_display, m_glxContext );
			XFree( m_fbConfig );

			if ( m_xWindow )
			{
				XDestroyWindow( m_display, m_xWindow );
			}

			if ( m_map )
			{
				XFreeColormap( m_display, m_map );
			}

			if ( m_display )
			{
				XCloseDisplay( m_display );
			}
		}

		inline VkXlibSurfaceCreateInfoKHR getCreateInfo()const
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

	private:
		Colormap m_map{ 0 };
		Display * m_display{ nullptr };
		Window m_xWindow{ 0 };
		GLXFBConfig m_fbConfig{ nullptr };
		GLXContext m_glxContext;
	};

#endif

	VkPhysicalDeviceMemoryProperties const Instance::m_memoryProperties = []()
	{
		VkPhysicalDeviceMemoryProperties result;
		result.memoryHeapCount = 2u;
		// Emulate one device local heap
		result.memoryHeaps[0] = { ~( 0ull ), VK_MEMORY_HEAP_DEVICE_LOCAL_BIT };
		// and one host visible heap
		result.memoryHeaps[1] = { ~( 0ull ), 0u };

		result.memoryTypeCount = 2u;
		// Emulate all combinations of device local memory types
		// and all combinations of host visible memory types
		result.memoryTypes[0] = { VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, 0u };
		result.memoryTypes[1] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, 1u };

		return result;
	}();

	std::string convert( const char * ptr )
	{
		return std::string{ ptr };
	}

	StringArray convert( const char * const * ptr
		, uint32_t count )
	{
		StringArray result;

		for ( auto it = ptr; it < ptr + count; ++it )
		{
			result.push_back( convert( *it ) );
		}

		return result;
	}

	Instance::Instance( VkInstanceCreateInfo createInfo )
		: m_flags{ createInfo.flags }
		, m_enabledLayerNames{ convert( createInfo.ppEnabledLayerNames, createInfo.enabledLayerCount ) }
		, m_enabledExtensions{ convert( createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount ) }
		, m_dummyWindow{ new RenderWindow }
	{
		m_extensions.initialise();
		m_features.hasTexBufferRange = m_extensions.find( ARB_texture_buffer_range );
		m_features.hasImageTexture = m_extensions.find( ARB_shader_image_load_store );
		m_features.hasBaseInstance = m_extensions.find( ARB_base_instance );
		m_features.hasClearTexImage = m_extensions.find( ARB_clear_texture );
		m_features.hasComputeShaders = m_extensions.find( ARB_compute_shader );
		m_features.supportsPersistentMapping = m_extensions.find( ARB_buffer_storage );
		auto it = std::find_if( m_enabledLayerNames.begin()
			, m_enabledLayerNames.end()
			, []( std::string const & lookup )
			{
				return lookup == "validation";
			} );
		m_validationEnabled = it != m_enabledLayerNames.end();
		m_context = Context::create( get( this )
			, m_dummyWindow->getCreateInfo()
			, nullptr );
	}

	Instance::~Instance()
	{
		m_context.reset();
		delete m_dummyWindow;
	}

	VkPhysicalDeviceArray Instance::enumeratePhysicalDevices()const
	{
		RenderWindow dummyWindow;
		VkPhysicalDeviceArray result;
		result.emplace_back( VkPhysicalDevice( new PhysicalDevice{ VkInstance( this ) } ) );
		return result;
	}

	std::array< float, 16 > Instance::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/glFrustum.xml)
		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left ),
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[8] = ( right + left ) / ( right - left );
		result[9] = ( top + bottom ) / ( top - bottom );
		result[10] = ( zNear + zFar ) / ( zNear - zFar );
		result[11] = float( -1 );
		result[13] = ( float( -2 ) * zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml)
		std::array< float, 16 > result{ 0.0f };
		float const tanHalfFovy = tan( radiansFovY / float( 2 ) );
		result[0] = float( 1 / ( tanHalfFovy * aspect ) );
		result[5] = float( 1 / tanHalfFovy );
		result[10] = float( -( zFar + zNear ) / ( zFar - zNear ) );
		result[11] = float( -1 );
		result[14] = float( -2 * zFar * zNear / ( zFar - zNear ) );

		return result;
	}

	std::array< float, 16 > Instance::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml)
		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 2 ) / ( right - left );
		result[5] = float( 2 ) / ( top - bottom );
		result[10] = float( -2 ) / ( zFar - zNear );
		result[12] = -( right + left ) / ( right - left );
		result[13] = -( top + bottom ) / ( top - bottom );
		result[14] = -( zFar + zNear ) / ( zFar - zNear );
		result[15] = 1.0f;

		return result;
	}

	std::array< float, 16 > Instance::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		float const range = tan( radiansFovY / float( 2 ) ) * zNear;
		float const left = -range * aspect;
		float const right = range * aspect;
		float const bottom = -range;
		float const top = range;

		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[10] = -float( 1 );
		result[11] = -float( 1 );
		result[14] = -float( 2 ) * zNear;
		return result;
	}

	void Instance::registerDebugMessageCallback( VkDebugReportCallbackEXT report
		, PFNGLDEBUGPROC callback
		, void * userParam )const
	{
		m_debugCallbacks.push_back( { report, callback, userParam } );
	}

	void Instance::registerDebugMessageCallbackAMD( VkDebugReportCallbackEXT report
		, PFNGLDEBUGAMDPROC callback
		, void * userParam )const
	{
		m_debugAMDCallbacks.push_back( { report, callback, userParam } );
	}

	void Instance::reportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		for ( auto & callback : m_debugCallbacks )
		{
			get( callback.debugReport )->report( flags
				, objectType
				, object
				, location
				, messageCode
				, pLayerPrefix
				, pMessage );
		}

		for ( auto & callback : m_debugAMDCallbacks )
		{
			get( callback.debugReport )->report( flags
				, objectType
				, object
				, location
				, messageCode
				, pLayerPrefix
				, pMessage );
		}
	}
}
