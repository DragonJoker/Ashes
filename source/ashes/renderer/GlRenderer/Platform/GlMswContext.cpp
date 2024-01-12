/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlMswContext.hpp"

#if _WIN32

#include "ashesgl_api.hpp"

#include <iostream>
#include <sstream>

#if _WIN32
#	define GLAPIENTRY __stdcall
#else
#	define GLAPIENTRY
#endif

#pragma warning( disable: 4191 )

namespace ashes::gl
{
	PFN_vkVoidFunction getFunction( char const * const name )
	{
		return reinterpret_cast< PFN_vkVoidFunction >( wglGetProcAddress( name ) );
	}

	namespace
	{
		enum class ContextFlag
		{
			GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = 0x0001,
			GL_CONTEXT_FLAG_DEBUG_BIT = 0x0002,
		};

		enum class ContextMaskFlag
		{
			GL_CONTEXT_CORE_PROFILE_BIT = 0x00000001,
		};

		enum class ContextParameter
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091,
			WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092,
			WGL_CONTEXT_FLAGS_ARB = 0x2094,
			WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126,
		};

		enum class PixelFormatParameter
		{
			WGL_NUMBER_PIXEL_FORMATS_ARB = 0x2000,
			WGL_DRAW_TO_WINDOW_ARB = 0x2001,
			WGL_ACCELERATION_ARB = 0x2003,
			WGL_NEED_PALETTE_ARB = 0x2004,
			WGL_NEED_SYSTEM_PALETTE_ARB = 0x2005,
			WGL_TRANSPARENT_ARB = 0x200A,
			WGL_SUPPORT_OPENGL_ARB = 0x2010,
			WGL_PIXEL_TYPE_ARB = 0x2013,
			WGL_COLOR_BITS_ARB = 2014,
			WGL_RED_BITS_ARB = 0x2015,
			WGL_GREEN_BITS_ARB = 0x2017,
			WGL_BLUE_BITS_ARB = 0x2019,
			WGL_NO_ACCELERATION_ARB = 0x2025,
			WGL_GENERIC_ACCELERATION_ARB = 0x2026,
			WGL_FULL_ACCELERATION_ARB = 0x2027,
			WGL_ALPHA_BITS_ARB = 0x201B,
			WGL_DEPTH_BITS_ARB = 0x2022,
			WGL_STENCIL_BITS_ARB = 0x2023,
			WGL_TYPE_RGBA_ARB = 0x202B,
			WGL_DRAW_TO_PBUFFER_ARB = 0x202D,
			WGL_MAX_PBUFFER_PIXELS_ARB = 0x202E,
			WGL_MAX_PBUFFER_WIDTH_ARB = 0x202F,
			WGL_MAX_PBUFFER_HEIGHT_ARB = 0x2030,
			WGL_PBUFFER_LARGEST_ARB = 0x2033,
			WGL_PBUFFER_WIDTH_ARB = 0x2034,
			WGL_PBUFFER_HEIGHT_ARB = 0x2035,
			WGL_PBUFFER_LOST_ARB = 0x2036,
		};

#if !defined( NDEBUG )

		constexpr int GL_CONTEXT_CREATION_DEFAULT_FLAGS =  GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		constexpr int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		constexpr int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		constexpr int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif

		std::string & replace( std::string & text
			, std::string const & lookup
			, std::string const & replacement )
		{
			std::string result;
			std::size_t currentPos = 0;
			std::size_t pos = 0;

			while ( ( pos = text.find( lookup, currentPos ) ) != std::string::npos )
			{
				result.append( text.substr( currentPos, pos - currentPos ) );
				result.append( replacement );
				currentPos = pos + lookup.size();
			}

			if ( currentPos != text.size() )
			{
				result.append( text.substr( currentPos, pos - currentPos ) );
			}

			text = result;
			return text;
		}

		std::string getLastErrorText()
		{
			uint32_t errorCode = ::GetLastError();
			std::stringstream stream;
			std::locale loc{ "C" };
			stream.imbue( loc );
			stream << "0x" << std::hex << errorCode;

			if ( errorCode != ERROR_SUCCESS )
			{
				LPWSTR errorText = nullptr;

				if ( ::FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
					, nullptr
					, errorCode
					, 0
					, LPWSTR( &errorText )
					, 0
					, nullptr ) != 0 )
				{
					int length = WideCharToMultiByte( CP_UTF8, 0u, errorText, -1, nullptr, 0u, nullptr, nullptr );

					if ( length > 0 )
					{
						std::string converted( size_t( length ), 0 );
						WideCharToMultiByte( CP_UTF8, 0u, errorText, -1, converted.data(), length, nullptr, nullptr );
						replace( converted, "\r", std::string{} );
						replace( converted, "\n", std::string{} );
						stream << " (" << converted.c_str() << ")";
					}
				}
				else
				{
					stream << " (Unable to retrieve error text)";
				}
			}
			else
			{
				stream << " (No error)";
			}

			auto result = stream.str();
			replace( result, "\r", "" );
			replace( result, "\n", "" );
			return result;
		}

		HWND createFullscreenWindow( VkDisplaySurfaceCreateInfoKHR const & createInfo )
		{
			auto hInstance = ::GetModuleHandleA( nullptr );
			auto name = "fullscreen_" + std::to_string( uint64_t( createInfo.displayMode ) );
			auto classId = win::registerClass( hInstance
				, name );
			return win::createWindow( hInstance
				, classId
				, name
				, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE
				, createInfo.imageExtent );
		}
	}

	struct VkStructure
	{
		VkStructureType sType;
		VkStructure const * pNext;
	};

	VkWin32PixelFormatDescriptorASH const * getPfd( VkStructure const * rhs )
	{
		VkWin32PixelFormatDescriptorASH const * result{ nullptr };

		if ( rhs )
		{
			if ( rhs->sType == VK_STRUCTURE_TYPE_WIN32_PIXEL_FORMAT_DESCRIPTOR_ASH )
			{
				result = reinterpret_cast< VkWin32PixelFormatDescriptorASH const * >( rhs );
			}
			else
			{
				result = getPfd( rhs->pNext );
			}
		}

		return result;
	}

	MswContext::MswContext( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, win32CreateInfo{ std::move( createInfo ) }
		, m_pfd{ getPfd( reinterpret_cast< VkStructure const * >( &win32CreateInfo ) ) }
		, m_hWnd{ createInfo.hwnd }
		, m_hDC{ ::GetDC( m_hWnd ) }
		, m_mainContext{ static_cast< MswContext const * >( mainContext ) }
	{
	}

	MswContext::MswContext( VkInstance instance
		, VkDisplaySurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, displayCreateInfo{ std::move( createInfo ) }
		, m_pfd{ getPfd( reinterpret_cast< VkStructure const * >( &displayCreateInfo ) ) }
		, m_hWnd{ createFullscreenWindow( displayCreateInfo ) }
		, m_hDC{ ::GetDC( m_hWnd ) }
		, m_mainContext{ static_cast< MswContext const * >( mainContext ) }
	{
	}

	MswContext::~MswContext()noexcept
	{
		if ( displayCreateInfo.sType )
		{
			ChangeDisplaySettings( nullptr, 0 );
		}

		if ( m_hContext )
		{
			wglDeleteContext( m_hContext );
		}

		::ReleaseDC( m_hWnd, m_hDC );

		if ( displayCreateInfo.sType )
		{
			::DestroyWindow( m_hWnd );
		}
	}

	void MswContext::preInitialise( int reqMajor, int reqMinor )
	{
		doSelectFormat();
		auto & extensions = get( instance )->getExtensions();
		m_major = std::max( reqMajor, int( extensions.getMajor() ) );
		m_minor = std::max( reqMinor, int( extensions.getMinor() ) );
		m_hContext = wglCreateContext( m_hDC );

		if ( !m_hContext )
		{
			throw ashes::BaseException{ "Couldn't create preliminary context: " + getLastErrorText() };
		}

		doLoadSystemFunctions();
	}

	void MswContext::postInitialise()
	{
		if ( displayCreateInfo.sType )
		{
			doSetFullscreen();
		}

		doCreateModernContext();
	}

	void MswContext::doSetFullscreen()const
	{
		if ( displayCreateInfo.transform != VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR )
		{
			DEVMODE dm{};
			dm.dmSize = sizeof( DEVMODE );
			dm.dmFields = DM_DISPLAYORIENTATION;

			if ( displayCreateInfo.transform == VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR )
			{
				dm.dmDisplayOrientation = DMDO_DEFAULT;
			}
			else if ( displayCreateInfo.transform == VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR )
			{
				dm.dmDisplayOrientation = DMDO_90;
			}
			else if ( displayCreateInfo.transform == VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR )
			{
				dm.dmDisplayOrientation = DMDO_180;
			}
			else if ( displayCreateInfo.transform == VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR )
			{
				dm.dmDisplayOrientation = DMDO_270;
			}

#if !defined( NDEBUG )
			auto res = ChangeDisplaySettings( &dm, 0 );
			assert( res == DISP_CHANGE_SUCCESSFUL );
#else
			ChangeDisplaySettings( &dm, 0 );
#endif
		}
	}

	void MswContext::enable()const
	{
		wglMakeCurrent( m_hDC, m_hContext );
	}

	void MswContext::disable()const noexcept
	{
		wglMakeCurrent( nullptr, nullptr );
	}

	void MswContext::swapBuffers()const
	{
		::SwapBuffers( m_hDC );
	}

	VkExtent2D MswContext::getExtent()const
	{
		if ( displayCreateInfo.sType )
		{
			return displayCreateInfo.imageExtent;
		}

		VkExtent2D result{};

		if ( RECT rect;
			::GetClientRect( m_hWnd, &rect ) )
		{
			result.width = uint32_t( rect.right - rect.left );
			result.height = uint32_t( rect.bottom - rect.top );
		}

		return result;
	}

	void MswContext::doSelectFormat()
	{
		PIXELFORMATDESCRIPTOR pfd = ( m_pfd
			? m_pfd->pfd
			: []()
			{
				PIXELFORMATDESCRIPTOR result{};
				result.nSize = sizeof( PIXELFORMATDESCRIPTOR );
				result.nVersion = 1;
				result.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
				result.iPixelType = PFD_TYPE_RGBA;
				result.iLayerType = PFD_MAIN_PLANE;
				result.cColorBits = 24;
				result.cRedBits = 8;
				result.cGreenBits = 8;
				result.cBlueBits = 8;
				result.cAlphaBits = 8;
				result.cDepthBits = 0;
				result.cStencilBits = 0;
				return result;
			}() );

		int pixelFormats = ::ChoosePixelFormat( m_hDC, &pfd );

		if ( !pixelFormats )
		{
			throw ashes::BaseException{ "Couldn't choose a pixel format: " + getLastErrorText() };
		}

		if ( !::SetPixelFormat( m_hDC, pixelFormats, &pfd ) )
		{
			throw ashes::BaseException{ "Couldn't set pixel format: " + getLastErrorText() };
		}
	}

	void MswContext::doLoadSystemFunctions() try
	{
		enable();
		std::stringstream errStream;

		if ( !getFunction( "wglCreateContextAttribsARB", wglCreateContextAttribsARB, errStream ) )
		{
			throw ashes::BaseException{ "Couldn't retrieve wglCreateContextAttribsARB: " + errStream.str() };
		}

		getFunction( "wglSwapIntervalEXT", wglSwapIntervalEXT, errStream );
		disable();
	}
	catch ( std::exception & )
	{
		disable();

		if ( m_hContext )
		{
			wglDeleteContext( m_hContext );
		}

		throw;
	}

	void MswContext::doCreateModernContext() try
	{
		std::vector< int > attribList
		{
			int( ContextParameter::WGL_CONTEXT_MAJOR_VERSION_ARB ), m_major,
			int( ContextParameter::WGL_CONTEXT_MINOR_VERSION_ARB ), m_minor,
			int( ContextParameter::WGL_CONTEXT_FLAGS_ARB ), GL_CONTEXT_CREATION_DEFAULT_FLAGS,
			int( ContextParameter::WGL_CONTEXT_PROFILE_MASK_ARB ), GL_CONTEXT_CREATION_DEFAULT_MASK,
			0
		};

		enable();

		auto hContext = wglCreateContextAttribsARB( m_hDC
			, ( m_mainContext
				? m_mainContext->m_hContext
				: nullptr )
			, attribList.data() );
		disable();
		wglDeleteContext( m_hContext );
		m_hContext = hContext;

		if ( !m_hContext )
		{
			std::stringstream error;
			error << "Failed to create an OpenGL " << m_major << "." << m_minor << " context (0x" << std::hex << ::glGetError() << ").";
			throw ashes::BaseException{ error.str() };
		}

		enable();

		if ( wglSwapIntervalEXT )
		{
			wglSwapIntervalEXT( 0 );
		}

		disable();
	}
	catch ( std::exception & )
	{
		disable();

		if ( m_hContext )
		{
			wglDeleteContext( m_hContext );
		}

		throw;
	}
}

#endif
