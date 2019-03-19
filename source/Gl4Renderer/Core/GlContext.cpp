#include "Core/GlContext.hpp"

#include "Core/GlPhysicalDevice.hpp"

#if ASHES_WIN32
#	include "Core/GlMswContext.hpp"
#elif ASHES_XLIB
#	include "Core/GlX11Context.hpp"
#endif

namespace gl_renderer
{
	Context::Context( PhysicalDevice const & gpu
		, ashes::Surface const & surface )
		: m_gpu{ gpu }
		, m_surface{ surface }
	{
	}

	Context::~Context()
	{
	}

	ContextPtr Context::create( PhysicalDevice const & gpu
		, ashes::Surface const & surface
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
#if defined( _WIN32 )
			result = std::make_unique< MswContext >( gpu, surface, mainContext );
#elif defined( __linux__ )
			result = std::make_unique< X11Context >( gpu, surface, mainContext );
#endif
		}
		catch ( std::exception & error )
		{
			ashes::Logger::logError( error.what() );
		}

		return result;
	}
}
