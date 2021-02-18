/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextImpl.hpp"

#if defined( __APPLE__ )
#include "CGlView.hh"

namespace ashes::gl
{
	void checkCGLErrorCode( CGLError error, std::string const & name );

	class CoreContext
		: public ContextImpl
	{
	public:
		CoreContext( VkInstance instance
			, VkMacOSSurfaceCreateInfoMVK createInfo
			, ContextImpl const * mainContext );
		CoreContext( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~CoreContext();

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;

	private:
		VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo{};
		VkDisplaySurfaceCreateInfoKHR displayCreateInfo{};
		GLView m_glView{};
		CGLContextObj m_cglContext{ nullptr };
		int m_minor{ 0 };
		int m_major{ 0 };
		CoreContext const * m_mainContext{ nullptr };
	};
}

#endif
