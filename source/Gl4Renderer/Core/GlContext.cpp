#include "Core/GlContext.hpp"

#if ASHES_WIN32
#	include "Core/GlMswContext.hpp"
#elif ASHES_XLIB
#	include "Core/GlX11Context.hpp"
#endif

namespace gl_renderer
{
	Context::Context( Instance const & instance )
		: m_instance{ instance }
	{
	}

	Context::~Context()
	{
	}

	ContextPtr Context::create( Instance const & instance
		, ashes::WindowHandle const & handle
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
#if defined( _WIN32 )
			result = std::make_unique< MswContext >( instance, handle, mainContext );
#elif defined( __linux__ )
			result = std::make_unique< X11Context >( instance, handle, mainContext );
#endif
		}
		catch ( std::exception & error )
		{
			ashes::Logger::logError( error.what() );
		}

		return result;
	}
}
