#include "Core/GlContext.hpp"

#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlRenderer.hpp"

#if ASHES_WIN32
#	include "Core/GlMswContext.hpp"
#elif ASHES_XLIB
#	include "Core/GlX11Context.hpp"
#endif

namespace gl_renderer
{
	Context::Context( PhysicalDevice const & gpu
		, ashes::Connection const & connection )
		: m_gpu{ gpu }
		, m_connection{ connection }
	{
	}

	Context::~Context()
	{
	}

	ContextPtr Context::create( PhysicalDevice const & gpu
		, ashes::Connection const & connection
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
			ashes::Logger::logError( error.what() );
		}

		return result;
	}
}
