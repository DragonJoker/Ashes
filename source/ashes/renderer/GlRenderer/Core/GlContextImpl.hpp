/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class ContextImpl
	{
	protected:
		ContextImpl( VkInstance instance
			, VkExtent2D const & extent = {} )
			: instance{ instance }
			, extent{ extent }
		{
		}

	public:
		virtual ~ContextImpl() = default;

		virtual void preInitialise( int major, int minor ) = 0;
		virtual void postInitialise() = 0;
		virtual void enable()const = 0;
		virtual void disable()const = 0;
		virtual void swapBuffers()const = 0;

#ifdef _WIN32
		static ContextImplPtr create( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
#elif __linux__
		static ContextImplPtr create( VkInstance instance
			, VkXlibSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		static ContextImplPtr create( VkInstance instance
			, VkXcbSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		static ContextImplPtr create( VkInstance instance
			, VkWaylandSurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
#elif __APPLE__
		static ContextImplPtr create( VkInstance instance
			, VkMacOSSurfaceCreateInfoMVK createInfo
			, ContextImpl const * mainContext );
#endif
#ifdef VK_KHR_display
		static ContextImplPtr create( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
#endif

		VkInstance instance;
		VkExtent2D extent;
	};
}
