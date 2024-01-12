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
		explicit ContextImpl( VkInstance instance )
			: instance{ instance }
		{
		}

	public:
		virtual ~ContextImpl()noexcept = default;

		virtual void preInitialise( int major, int minor ) = 0;
		virtual void postInitialise() = 0;
		virtual void enable()const = 0;
		virtual void disable()const noexcept = 0;
		virtual void swapBuffers()const = 0;
		virtual VkExtent2D getExtent()const = 0;

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
	};
}
