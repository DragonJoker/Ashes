#include "Core/GlContext.hpp"

#if VK_USE_PLATFORM_WIN32_KHR
#	include "Core/GlMswContext.hpp"
#elif VK_USE_PLATFORM_XLIB_KHR
#	include "Core/GlX11Context.hpp"
#else
#	error "Unsupported surface type"
#endif

#include "ashesgl4_api.hpp"

#include <iostream>

namespace ashes::gl4
{
	Context::Context( VkInstance instance )
		: m_instance{ instance }
	{
	}

	Context::~Context()
	{
	}

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkAndroidSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_FUCHSIA )

	ContextPtr Context::create( VkInstance instance
		, VkImagePipeSurfaceCreateInfoFUCHSIA createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_IOS_MVK )

	ContextPtr Context::create( VkInstance instance
		, VkIOSSurfaceCreateInfoMVK createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_MACOS_MVK )

	ContextPtr Context::create( VkInstance instance
		, VkMacOSSurfaceCreateInfoMVK createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_VI_NN )

	ContextPtr Context::create( VkInstance instance
		, VkViSurfaceCreateInfoNN createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_XCB_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkXcbSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkXlibSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
			result = std::make_unique< X11Context >( instance
				, std::move( createInfo )
				, mainContext );
		}
		catch ( std::exception & error )
		{
			std::cerr << error.what() << std::endl;
		}

		return result;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
			result = std::make_unique< X11Context >( instance
				, surface
				, mainContext );
		}
		catch ( std::exception & error )
		{
			std::cerr << error.what() << std::endl;
		}

		return result;
	}

#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkWaylandSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		std::cerr << "Not implemented" << std::endl;
		return nullptr;
	}

#elif defined( VK_USE_PLATFORM_WIN32_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
			result = std::make_unique< MswContext >( instance
				, std::move( createInfo )
				, mainContext );
		}
		catch ( std::exception & error )
		{
			std::cerr << error.what() << std::endl;
		}

		return result;
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		ContextPtr result;

		try
		{
			result = std::make_unique< MswContext >( instance
				, surface
				, mainContext );
		}
		catch ( std::exception & error )
		{
			std::cerr << error.what() << std::endl;
		}

		return result;
	}

#endif
}
