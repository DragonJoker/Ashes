/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/GlMswContext.hpp"

#if _WIN32

#include <iostream>
#include <sstream>

#if _WIN32
#	define GLAPIENTRY __stdcall
#else
#	define GLAPIENTRY
#endif

namespace ashes::gl
{
	PFN_vkVoidFunction getFunction( char const * const name )
	{
		return reinterpret_cast< PFN_vkVoidFunction >( wglGetProcAddress( name ) );
	}

	namespace
	{
		enum ContextFlag
		{
			GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = 0x0001,
			GL_CONTEXT_FLAG_DEBUG_BIT = 0x0002,
		};

		enum ContextMaskFlag
		{
			GL_CONTEXT_CORE_PROFILE_BIT = 0x00000001,
		};

		enum ContextParameter
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091,
			WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092,
			WGL_CONTEXT_FLAGS_ARB = 0x2094,
			WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126,
		};

#if !defined( NDEBUG )

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS =  GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif
	}

	MswContext::MswContext( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo
		, ContextImpl const * mainContext )
		: ContextImpl{ instance }
		, createInfo{ std::move( createInfo ) }
		, m_hDC{ ::GetDC( createInfo.hwnd ) }
		, m_mainContext{ static_cast< MswContext const * >( mainContext ) }
	{
	}

	MswContext::~MswContext()
	{
		try
		{
			if ( m_hContext )
			{
				wglDeleteContext( m_hContext );
			}

			::ReleaseDC( createInfo.hwnd, m_hDC );
		}
		catch ( ... )
		{
		}
	}

	void MswContext::preInitialise( int major, int minor )
	{
		if ( !doSelectFormat() )
		{
			throw std::runtime_error{ "Couldn't find an appropriate pixel format." };
		}

		m_major = major;
		m_minor = minor;
		m_hContext = wglCreateContext( m_hDC );

		if ( !m_hContext )
		{
			throw std::runtime_error{ "Couldn't create preliminary context." };
		}
	}

	void MswContext::postInitialise()
	{
		if ( !doCreateModernContext() )
		{
			throw std::runtime_error{ "Couldn't create modern context." };
		}
	}

	void MswContext::enable()const
	{
		wglMakeCurrent( m_hDC, m_hContext );
	}

	void MswContext::disable()const
	{
		wglMakeCurrent( nullptr, nullptr );
	}

	void MswContext::swapBuffers()const
	{
		::SwapBuffers( m_hDC );
	}

	bool MswContext::doSelectFormat()
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

	bool MswContext::doCreateModernContext()
	{
		bool result = false;

		try
		{
			using PFN_wglCreateContextAttribsARB = HGLRC(*)( HDC hDC, HGLRC hShareContext, int const * attribList );
			using PFN_wglSwapIntervalEXT = BOOL( GLAPIENTRY * )( int );
			PFN_wglCreateContextAttribsARB glCreateContextAttribs;
			PFN_wglSwapIntervalEXT glSwapInterval;

			std::vector< int > attribList
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, m_major,
				WGL_CONTEXT_MINOR_VERSION_ARB, m_minor,
				WGL_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
				WGL_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
				0
			};

			enable();

			if ( !getFunction( "wglCreateContextAttribsARB", glCreateContextAttribs ) )
			{
				disable();
				wglDeleteContext( m_hContext );
				throw std::runtime_error{ "Couldn't retrieve wglCreateContextAttribsARB" };
			}

			auto hContext = glCreateContextAttribs( m_hDC
				, ( m_mainContext
					? m_mainContext->m_hContext
					: nullptr )
				, attribList.data() );
			disable();
			wglDeleteContext( m_hContext );
			m_hContext = hContext;
			result = m_hContext != nullptr;

			if ( !result )
			{
				std::stringstream error;
				error << "Failed to create an OpenGL " << m_major << "." << m_minor << " context (0x" << std::hex << ::glGetError() << ").";
				throw std::runtime_error{ error.str() };
			}

			enable();
			if ( getFunction( "wglSwapIntervalEXT", glSwapInterval ) )
			{
				glSwapInterval( 0 );
			}
			disable();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = false;
		}
		catch ( ... )
		{
			result = false;
		}

		return result;
	}
}

#endif
