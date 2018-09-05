#include "Core/GlContext.hpp"

#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlRenderer.hpp"

#if RENDERLIB_WIN32
#	include "GlMswContext.hpp"
#elif RENDERLIB_XLIB
#	include "GlX11Context.hpp"
#endif

namespace gl_renderer
{
	Context::Context( PhysicalDevice const & gpu
		, renderer::Connection const & connection )
		: m_gpu{ gpu }
		, m_connection{ connection }
	{
	}

	Context::~Context()
	{
	}

	ContextPtr Context::create( PhysicalDevice const & gpu
		, renderer::Connection const & connection
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
#if defined( _WIN32 )
			result = std::make_unique< MswContext >( gpu, connection, mainContext );
#elif defined( __linux__ )
			result = std::make_unique< X11Context >( gpu, connection, mainContext );
#endif
		}
		catch ( std::exception & error )
		{
			renderer::Logger::logError( error.what() );
		}

		return result;
	}
}
