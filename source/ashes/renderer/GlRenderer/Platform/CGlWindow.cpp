/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Platform/CGlWindow.hpp"

#if defined( __APPLE__ )

namespace ashes::gl
{
	RenderWindow::RenderWindow( int major
		, int minor
		, std::string const & name )
	{
		try
		{
			std::vector< CGLPixelFormatAttribute > attributes
			{
				kCGLPFAAccelerated,   // no software rendering
				kCGLPFAOpenGLProfile, // core profile with the version stated below
				(CGLPixelFormatAttribute) kCGLOGLPVersion_3_2_Core,
				(CGLPixelFormatAttribute) 0
			};
			CGLPixelFormatObj pix;
			int num; // stores the number of possible pixel formats

			auto errorCode = CGLChoosePixelFormat( attributes.data(), &pix, &num );
			checkCGLErrorCode( errorCode, "CGLChoosePixelFormat" );

			errorCode = CGLCreateContext( pix
				, nullptr
				, &m_cglContext ); // second parameter can be another context for object sharing
			checkCGLErrorCode( errorCode, "CGLCreateContext" );

			CGLDestroyPixelFormat( pix );

			errorCode = CGLSetCurrentContext( m_cglContext );
			checkCGLErrorCode( errorCode, "CGLSetCurrentContext - m_cglContext" );
		}
		catch ( std::exception & exc )
		{
			if ( m_cglContext )
			{
				CGLDestroyContext( m_cglContext );
			}

			throw;
		}
	}

	RenderWindow::~RenderWindow()
	{
		auto errorCode = CGLSetCurrentContext( nullptr );
		checkCGLErrorCode( errorCode, "CGLSetCurrentContext - nullptr" );
		CGLDestroyContext( m_cglContext );
	}

	VkMacOSSurfaceCreateInfoMVK RenderWindow::getCreateInfo()const
	{
		return
		{
			VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
			nullptr,
			0u,
			nullptr,
		};
	}
}

#endif
