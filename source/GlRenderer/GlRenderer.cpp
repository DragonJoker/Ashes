#include "GlRenderer.hpp"

#include "GlConnection.hpp"
#include "GlDevice.hpp"

namespace gl_renderer
{
	Renderer::Renderer()
		: renderer::Renderer{}
	{
	}

	Renderer::~Renderer()
	{
	}

	renderer::DevicePtr Renderer::createDevice( renderer::ConnectionPtr && connection )const
	{
		renderer::DevicePtr result;

		try
		{
			result = std::make_unique< Device >( *this, std::move( connection ) );
		}
		catch ( std::exception & exc )
		{
			std::cerr << "Could not initialise logical device:\n" << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Could not initialise logical device:\nUnknown error" << std::endl;
		}

		return result;
	}

	renderer::ConnectionPtr Renderer::createConnection( uint32_t deviceIndex
		, renderer::WindowHandle && handle )const
	{
		return std::make_unique< Connection >( *this
			, deviceIndex
			, std::move( handle ) );
	}
}
